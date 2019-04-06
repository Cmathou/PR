#include "c8051F020.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "servo_H.h"
#include "ringB/UART0_RingBuffer_lib.h"
#include "ringB/UART1_RingBuffer_lib.h"

static char xdata D_nbr = 0;


int process(char* cmd_str) {
	//char xdata retourC[8];
	//int xdata retourI;
	char cmd[4] = "\0";
	char param1[7] = "\0";
	char param2[7] = "\0";
	char param3[7] = "\0";
	char param4[7] = "\0";
	sscanf(cmd_str, "%s %s %s %s %s", cmd, param1, param2, param3, param4);
	
	if (strcmp(cmd, "Q") == 0) {	//arret urgence
		//TODO
		return 0;
	}
	
	if (strcmp(cmd, "D") == 0) {	//debut epreuve
		D(param1);
		return 0;
	}
	
	if (D_nbr != 0) {
		
		if (strcmp(cmd, "L") == 0) {	//lumiere
			//LumiereDegra(param1, param2, param3, param4);
			valid();
			return 0;
		}
		
		if (strcmp(cmd, "A") == 0) {	//avance
			AB(param1, '0');
			return 0;
		}
		
		if (strcmp(cmd, "B") == 0) {	//recule
			AB(param1, '1');
			return 0;
		}
		
		if (strcmp(cmd, "TV") == 0) {		//vittesse par default
			TV(param1);
			return 0;
		}
		
		if (strcmp(cmd, "S") == 0) {		//stop les moteurs
			S();
			return 0;
		}
		
		if (strcmp(cmd, "RD") == 0) {		//rotation de 90° droite
			R(0, 0);
			return 0;
		}
		
		if (strcmp(cmd, "RG") == 0) {		//rotation de 90° gauche
			R(1, 0);
			return 0;
		}
		
		if (strcmp(cmd, "RC") == 0) {		//rotation de 180°
			if (strcmp(param1, "D") == 0) {
				R(0, 1);
			} else {
				if (strcmp(param1, "G") == 0) {
					R(1, 1);
				}
				else {
					invalid();
				}
			}
			return 0;
		}
		
		if (strcmp(cmd, "CS") == 0) {		//servo H
			if (ServoHorizontal(cmd, param1, param2) == 1) {
				valid();
			}
			else {
				invalid();
			}
			return 0;
		}
		/*
		if (strcmp(cmd, "MOU") == 0) {		//mesure distance
			retourI = mesure_distance(cmd, param1);
			if (retourI != -1) {
				sprintf(retourC, "%d", retourI);
				serOutstring("Distance : ");
				serOutstring(retourC);
				serOutstring("\r\n");
				valid();
			}
			else {
				invalid();
			}
			return 0;
		}*/
	}
	
	if (strcmp(cmd, "E") == 0) {		//fin d'epreuve
		D_nbr = 0;
		valid();
		return 0;
	}
		
	invalid();
	return 1;
}

////////////////////////////////////////////////////////
//interrupt function
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//functions
////////////////////////////////////////////////////////

void valid() {
	serOutstring(">\r\n");
}

void invalid() {
	serOutstring("#\r\n");
}

void D(char* param) {
	int xdata d = atoi(param);
	if (d > 0 && d < 9) {
		D_nbr = d;
		serOutstring("début de l'épreuve!\r\n");
		valid();
	}
	else {
		invalid();
	}
}
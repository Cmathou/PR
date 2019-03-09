#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "ringB/UART0_RingBuffer_lib.h"
#include "ringB/UART1_RingBuffer_lib.h"

int xdata dS = 20;
char xdata D_nbr = 0;

int process(char* cmd_str) {
	char cmd[4] = "\0";
	char param1[7] = "\0";
	char param2[7] = "\0";
	char param3[7] = "\0";
	sscanf(cmd_str, "%s %s %s %s", cmd, param1, param2, param3);
	
	if (strcmp(cmd, "Q") == 0) {	//arret urgence
		//TODO
		return 0;
	}
	
	if (strcmp(cmd, "D") == 0) {	//debut epreuve
		D(param1);
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
		serOutstring1("stop\r");
		valid();
		return 0;
	}
	
	if (strcmp(cmd, "RD") == 0) {		//rotation de 90° droite
		serOutstring1("mogo 1:20 2:-20\r");
		return 0;
	}
	
	if (strcmp(cmd, "RG") == 0) {		//rotation de 90° gauche
		serOutstring1("stop\r");
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
		valid();
	}
	else {
		invalid();
	}
}

void TV(char* param) {
	int xdata speed = atoi(param);
	if (5 <= speed && speed <= 100) {
		dS = speed;
		valid();
	}
	else {
		invalid();
	}
}

void AB(char* param, char sign) {
	int xdata speed = atoi(param);
	char xdata cmd[32] = "mogo 1:";
	if (strcmp(param, "") ==0) {
		sprintf(param, "%d", dS);
		if (sign == '1') {
			strcat(strcat(strcat(strcat(strcat(cmd, "mogo 1:-"), param), " 2:-"), param), "\r");
		}
		else {
			strcat(strcat(strcat(strcat(strcat(cmd, "mogo 1:"), param), " 2:"), param), "\r");
		}
		serOutstring1(cmd);
		valid();
	}
	else {
		if (5 <= speed && speed <= 100) {
			if (sign == '1') {
			strcat(strcat(strcat(strcat(strcat(cmd, "mogo 1:-"), param), " 2:-"), param), "\r");
		}
		else {
			strcat(strcat(strcat(strcat(strcat(cmd, "mogo 1:"), param), " 2:"), param), "\r");
		}
			serOutstring1(cmd);
			valid();
		}
		else {
			invalid();
		}
	}
}
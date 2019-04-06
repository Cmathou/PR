#include "c8051F020.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "servo_V.h"
#include "ringB/UART0_RingBuffer_lib.h"
#include "ringB/UART1_RingBuffer_lib.h"

int process(char* cmd_str) {
	char xdata retour[8];
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

	if (strcmp(cmd, "L") == 0) {	//lumiere
		LumiereDegra(param1, param2, param3, param4);
		valid();
		return 0;
	}
	
	if (strcmp(cmd, "CS") == 0) {		//servo
		if (setServoVertical(cmd, param1, param2) == 1) {
			valid();
		}
		else {
			invalid();
		}
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
#include "c8051F020.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "SPI.h"
#include "servo_V.h"
#include "laser.h"
#include "ringB/UART0_RingBuffer_lib.h"
#include "ringB/UART1_RingBuffer_lib.h"

int process(char* cmd_str) {
	char retour[8];
	char cmd[4] = "";
	char param1[7] = "";
	char param2[7] = "";
	char param3[7] = "";
	char param4[7] = "";
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
		if (!setServoVertical(cmd, param1, param2)) {
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
	//spistring(">\n");
}

void invalid() {
	//spistring("#\n");
}
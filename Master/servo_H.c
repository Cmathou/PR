#include "c8051F020.h"
#include <stdlib.h>
#include <string.h>


#include "ringB/UART0_RingBuffer_lib.h"
#include "ringB/UART1_RingBuffer_lib.h"

//cmd servomotors : CS H/V [angle]
//freq oscillator = 22.1184 MHz 
//1 us ~= 22 tick

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int timeHighServoHoriz = 0;
static unsigned int valHigh = 0;
static char * angle = "XX";

sbit OutServoH = P3 ^ 7; //à changer


/////////////////////////////////////////////////////////////////////////////////////////////////////////////


void enable_int () {
	EIE1 |= 0x08;
}


void configPCA0() {
    PCA0MD = 0x09;
    PCA0CPM0 = 0x49;
    CR = 1;
}

//cmd = "CS H -45"
char setServoHorizontal(char* cmd, char* param1, char* param2) {
	char ret = 0;
	char * angle = param2+2;
	if (strcmp(cmd, "CS") == 0) {
		if (strcmp(param1, "H") == 0) {                          // 'H' pour le servo horizontal, 'V' pour le Vertical
			timeHighServoHoriz = 1500 + atoi(angle) * 10;  //consigne = +/- 90 => timeHigh = [600; 2400] us

			if (timeHighServoHoriz < 600) {
					timeHighServoHoriz = 600;
			}
			if (timeHighServoHoriz > 2400) {
					timeHighServoHoriz = 2400;
			}

			valHigh = timeHighServoHoriz * 22;

			PCA0CPL0 = valHigh & 0xFF;
			PCA0CPH0 = (valHigh >> 8) & 0xFF;
			
			//serOutstring("");
			
			ret = 0;
		} 
		else {
			ret = 2;
		}
	} else {
		ret = 1;
	}
	return ret;
} 
	

void init_servoH(){
  configPCA0();
	enable_int();
	P3MDOUT = P3MDOUT | 0x80; //Passe le pin 3.7 en push-pull
	XBR2 = XBR2 | 0x40;
	setServoHorizontal("CS", "H", "0");
}

void ServoHorizontal() interrupt 9 {
    if (CCF0 == 1) {
        CCF0 = 0;
        OutServoH = 0;
    }
    if (CF == 1) {
        CF = 0;
        OutServoH = 1;
    }
}

//char * angleActuel(){
//	return angle;
//}


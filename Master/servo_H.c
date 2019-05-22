#include "c8051F020.h"
#include <stdlib.h>
#include <string.h>

//cmd servomotors : CS H/V [angle]
//freq oscillator = 22.1184 MHz 
//1 us ~= 22 tick

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int timeHighServoHoriz = 0;
unsigned int valTimer = 0;
unsigned int valHigh = 0;

sbit OutServoH = P3 ^ 7; //à changer


/////////////////////////////////////////////////////////////////////////////////////////////////////////////


void enable_int () {
	//EA = 1; // General interrupt enable
	//ET2 = 0x01; // TIMER2-OVFLW
	return;
}


void configTimer2(){
	RCLK0 = 0;//pas UART0
	TCLK0 = 0;//pas UART0
	CPRL2 = 0;
	CKCON |= 0x20;
	RCAP2H = 0x00; //reload value
	RCAP2L = 0x00;
	TR2 = 1; // Activation du timer 2
}


void init_servoH(){
	configTimer2();
	enable_int();
	P3MDOUT = P3MDOUT | 0x80; //Passe le pin 3.7 en push-pull
	XBR2 = XBR2 | 0x40;
}

//cmd = "CS H -45"
int ServoHorizontal(char* cmd, char* param1, char* param2){

	if(strcmp(cmd, "CS") == 0){
		if(strcmp(param1, "H") == 0){
	
			timeHighServoHoriz = 1500+atoi(param2)*10; //consigne = +/- 90 => timeHigh = [600; 2400] us
			
			if(timeHighServoHoriz < 600){
				timeHighServoHoriz = 600;
			}
			
			if(timeHighServoHoriz > 2400){
				timeHighServoHoriz = 2400;
			}
		}
	}
	
	valTimer = (TH2<<8)+TL2;
	valHigh = timeHighServoHoriz*22;
	
	if(valTimer < valHigh){
		OutServoH = 1;
	} else {
		OutServoH = 0;
	}
	return 1;
}
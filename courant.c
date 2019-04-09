#include "c8051F020.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

//cmd servomotors : CS H/V [angle]
//freq oscillator = 22.1184 MHz 
//1 us ~= 22 tick

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int j = 0;

sbit outServoV = P3 ^ 7;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void timerDelay(){ //delai de 10ms
	j = 0;
	for(j = 0; j < 25000; j++){
	}
}

void enable_int() {
	EA = 1; // General interrupt enable
	EIE1 |= 0x08;
}


void configADC0(){
	AMX0CF = 0x01; //AIN0 et AIN1 en differential
	AMX0SL = 0x00; //selection de AIN0/AIN1
	ADC0CF |= 0xF0; //internal gain = 1
	ADC0CN = 0xC0; //conversion par ecriture de 1 dans ad0busy
	REF0CN = 0x03; //ADC0 ref from VREF0
}


void initCourant(){
	configADC0();
	enable_int();

}

int mesureCourant(char* cmd){
	float courant = 0;
	float tension = 0;
	float shuntRes = 50; //mOhms
	float Vref = 2.43;
	
	if(strcmp(cmd, "MI") == 0){
		AD0INT = 0;
		AD0BUSY = 1;
		while(AD0INT == 0){}
		
		tension = ((((ADC0H & 0x0F) << 8) + ADC0L)*(1/Vref))*pow(2, 10);
			
		courant = tension/shuntRes;
	}
	
	return courant;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main(){
	WDTCN = 0xDE;   // Devalidation du watchdog 
	WDTCN = 0xAD;

	OSCXCN =  0xef; //configure external oscillator
	timerDelay();
	while(!(OSCXCN&(0x80))){}

	OSCICN = OSCICN | 0x08; //utilise l'oscillateur externe
	
	initCourant();
		
	
		
	while(1) 
	{
		mesureCourant("MI");
		timerDelay();
	}
}
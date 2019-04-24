#include "c8051F020.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "timers.h"

//cmd servomotors : CS H/V [angle]
//freq oscillator = 22.1184 MHz 
//1 us ~= 22 tick

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int j = 0;
sfr16 ADC0 = 0xBE;
float energie = 0;
static timeType timeCurrent = 1000;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void timerDelay(){ //delai de 10ms
	j = 0;
	for(j = 0; j < 25000; j++){
	}
}

void enable_int_courant() {
	EA = 1; // General interrupt enable
	EIE1 |= 0x08;
}


void configADC0(){
	AMX0CF = 0x00; //toutes les entrées indep
	AMX0SL = 0x00; //selection de AIN0/AIN1
	ADC0CF |= 0xF0; //internal gain = 1
	ADC0CN = 0xC0; //conversion par ecriture de 1 dans ad0busy
	REF0CN = 0x03; //ADC0 ref from VREF0 --> 2.4V
}


void initCourant(){
	configADC0();
	enable_int_courant();
}

char* mesureCourant(char* cmd){
	char ret[8];
	float courant = 0;
	float tension = 0;
	float shuntRes = 0.050; //Ohms
	float Vref = 2.40;
	
	if(strcmp(cmd, "MI") == 0){
		AD0INT = 0;
		AD0BUSY = 1;
		while(AD0INT == 0){}
		
		
			//tension = ((((ADC0H & 0x0F) << 8) + ADC0L)*Vref)/pow(2, 10);
			
		tension = (27*(ADC0*Vref)/(100*pow(2, 10)));
			
		courant = 1000*(tension/20)/shuntRes; //V -> mA, gain 20 (R = 2.7kOhm)
	}
	sprintf(ret, "%f", courant);
	return ret;
}

//////////////////////
float power(){
	float courantP = 0;
	float tensionP = 0;
	float puissance = 0;
	float shuntRes = 0.050; //Ohms
	float Vref = 2.40;
	
	AD0INT = 0;
	AD0BUSY = 1;
	while(AD0INT == 0){}
	
	
		//tension = ((((ADC0H & 0x0F) << 8) + ADC0L)*Vref)/pow(2, 10);
		
	tensionP = (27*(ADC0*Vref)/(100*pow(2, 10)));
		
	courantP = (tensionP/20)/shuntRes; //gain 20 (R = 2.7kOhm)
		
	puissance = tensionP*courantP;

	
	return puissance;
}

void timeEnergy() {
    if (timePass(timeCurrent)) {
			energie +=  1*power(); //1 seconde
			timeCurrent = getTime(1000);	
    }
}

char* mesureEnergie(char* cmd){
	char ret[8];
	if(strcmp(cmd, "ME") == 0){
		sprintf(ret, "%f", energie);
		return ret;	
	} else {
		return "0";
	}
	
}

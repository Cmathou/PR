//------------------------------------------------------------------------------------
// Detection_Obstacles_1.c
//------------------------------------------------------------------------------------
// DATE: 27/02/19
//
// Target: C8051F02x
// Tool chain: KEIL Microvision 4
//
//  NOMS: Prummel
//
//------------------------------------------------------------------------------------
#include "c8051F020.h"
#include "definitions.h"

//sbit LED = P1 ^ 6;
sbit MD = P3 ^ 3;

char cmd[20] = {0}; //chaine complete de commande
char typeCmd[5] = {0}; //premières lettres de la commande; indiquant son type

int distance; 

// Fonction d'initialisation des differents registres
void init_1(){
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	
	XBR2 = XBR2 | 0x40;
	P1MDOUT = P1MDOUT | 0x40;
	P3MDOUT = P3MDOUT | 0x08;
	
	ADC0CN = ADC0CN | 0x80;
	
	T4CON = 0x01; 
	
	EA = 1;
	EIE2 = EIE2 | 0x10;
}
// Delay pour le trigger du télémètre
void delay_10us(){
	int i;
	for(i=0;i<1;i++){}
}

void mesure_distance(typeCmd, cmd){
	char* consigne;
	char* currentPosConsigne;
	MD=1;
	if(typeCmd == "MD"){
		while(cmd[currentPosCmd] != ' '){ //relecture MD
			currentPosCmd++;
		}
		currentPosCmd++;
		if(cmd[currentPosCmd] == 'Back'){ //'Back' pour l'arriere
			MES_Dist_AR (void);
		}
		if(cmd[currentPosCmd] == 'Front'){ //'Front' pour l'arriere
			MES_Dist_AV (void);
		}
	}
	delay_10us();
	MD=0;
}

unsigned int MES_Dist_AV (void){
	
	while(cmd[currentPosCmd] != ' '){
		currentPosCmd++;
		consigne[currentPosCmd] = cmd[currentPosCmd];
	}
	delay_10us();
	currentPosCmd = 0;
}
	
unsigned int MES_Dist_AR (void){
	
	while(cmd[currentPosCmd] != ' '){
		currentPosCmd++;
		consigne[currentPosCmd] = cmd[currentPosCmd];
	}
	delay_10us();
	currentPosCmd = 0;
}

void mesure() interrupt 18{
	if((P3IF & 0x04)==0x04){
		T4CON = T4CON | 0x04;
		P3IF = P3IF & 0xFB;
	}
	else{
		distance = (TH4*256 + TL4)/12;
		P3IF = P3IF | 0x04;
	}
	P3IF = P3IF & 0xBF;
}


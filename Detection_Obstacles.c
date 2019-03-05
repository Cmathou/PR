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
//#include "definitions.h"

//sbit LED = P1 ^ 6;
sbit MD = P3 ^ 2;
sbit MD_A = P3 ^ 3;
sbit MD_AR = P3 ^ 4;

char cmd[20] = {0}; //chaine complete de commande
char typeCmd[5] = {0}; //premières lettres de la commande; indiquant son type
char currentPosCmd = 0; //iterateur sur la commande

int distance; 

// Fonction d'initialisation des differents registres
void init_1(){
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	
	XBR2 = XBR2 | 0x40;
	//P1MDOUT = P1MDOUT | 0x40;
	P3MDOUT = P3MDOUT | 0x1C;
	
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

void trigger(){
	MD=1;
	delay_10us();
	MD=0;
}

unsigned int MES_Dist_AV (void){ //Mesure distance avant
	
	if((P3IF & 0x04)==0x04){ //P3IF port 3 interrupt flag
		T4CON = T4CON | 0x04; //activation du timer4
		P3IF = P3IF & 0xFB;
		distance=0;
	}
	else{
		distance = (TH4*256 + TL4)/12; //conversion distance en cm
		P3IF = P3IF | 0x04;
	}
	P3IF = P3IF & 0xBF;
	return distance;
}
	
unsigned int MES_Dist_AR (void){ //Mesure distance arriere 
	
	if((P3IF & 0x08)==0x08){
		T4CON = T4CON | 0x04;
		P3IF = P3IF & 0xFB;
		distance=0;
	}
	else{
		distance = (TH4*256 + TL4)/12;
		P3IF = P3IF | 0x08;
	}
	P3IF = P3IF & 0xBF;
	return distance;
}

void mesure_distance(typeCmd,char* cmd){
	char* consigne;
	char* currentPosConsigne;
	if(typeCmd == "MD"){
		while(cmd[currentPosCmd] != ' '){ //relecture MD
			consigne[currentPosCmd] = cmd[currentPosCmd];
			currentPosCmd++;
		}
	
		if(cmd[currentPosCmd] == "Back"){ //'Back' pour l'arriere
			MD_A=1;
			MES_Dist_AR ();
		}
		if(cmd[currentPosCmd] == "Front"){ //'Front' pour l'arriere
			MD_AR=1;
			MES_Dist_AV ();
		}	}

	delay_10us();
	currentPosCmd = 0;
	MD_A=0;
}

//void main(){
//	WDTCN = 0xDE;   // Devalidation du watchdog 
//	WDTCN = 0xAD;

//	OSCXCN =  0xef; //configure external oscillator
//	delay_10us();
//	while(!(OSCXCN&(0x80))){}

//	OSCICN = OSCICN | 0x08; //utilise l'oscillateur externe
//	
//	init();
//  //SBUF0 = 'T';
//	while(1) 
//	{
//		// Gestion cmd
//		if(cmdRecue){
//			cmdRecue = 0;
//			mesure_distance(typeCmd,cmd);
//			
//			
//		}
//	}
//}




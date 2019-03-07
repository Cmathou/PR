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
#include <string.h>

//sbit LED = P1 ^ 6;
// MD = P3 ^ 2 ;
sbit MD_AV = P3 ^ 3;
sbit MD_AR = P3 ^ 4;

char cmd[20] = "MOU Front"; //chaine complete de commande
char typeCmd[5] = "MOU"; //premières lettres de la commande; indiquant son type
char currentPosCmd = 0; //iterateur sur la commande

int distance; 
int j;

// Fonction d'initialisation des differents registres
void init_1(){
	//WDTCN = 0xDE;
	//WDTCN = 0xAD;
	
	XBR2 = XBR2 | 0x40;
	//P1MDOUT = P1MDOUT | 0x40;
	P3MDOUT = P3MDOUT | 0x1C; //Gestion des entrées/sorties
	
	ADC0CN = ADC0CN | 0x80;
	
	T4CON = 0x01; 
	
	EA = 1;
	EIE2 = EIE2 | 0x10; //Autorisation interruption 6 (externe)
	P3IF = P3IF | 0x44; //front montant
}
// Delay pour le trigger du télémètre
void delay_10us(){ //13us
	int i;
	for(i=0;i<22;i++){}
}

void timerDelay(){ //delai de 10ms
	j = 0;
	for(j = 0; j < 25000; j++){
}
}

unsigned int MES_Dist_AV (void){ //Mesure distance avant
	delay_10us();
	
	MD_AV = 0;
	timerDelay();
	return distance;
}
	
unsigned int MES_Dist_AR (void){ //Mesure distance arriere 
	
	delay_10us();
	
	MD_AR = 0;
	timerDelay();
	return distance;
}
void mesure() interrupt 18{
	if((P3IF & 0x04)==0x04){ //P3IF port 3 interrupt flag
		TH4 = 0;
		TL4 = 0;
		T4CON = T4CON | 0x04; //activation du timer4
		P3IF = P3IF & 0xFB;
		distance=0;
	}
	else{
		T4CON = T4CON & 0xFB; //activation du timer4
		distance = (TH4*256 + TL4)/20; //conversion distance en cm
		P3IF = P3IF | 0x04;
	}
	P3IF = P3IF & 0xBF;
}
void mesure_distance(char* typeCmd,char* cmd){
	char* consigne;
//	char* currentPosConsigne;
	if(strcmp(typeCmd,"MOU")==0){
		while(cmd[currentPosCmd] != ' '){ //relecture MD
			consigne[currentPosCmd] = cmd[currentPosCmd];
			currentPosCmd++;
		}
		currentPosCmd++;
		if(cmd[currentPosCmd] == "B"){ //'Back' pour l'arriere
			MD_AR=1;
			MES_Dist_AR ();
		}
		if(cmd[currentPosCmd] == 'F'){ //'Front' pour l'arriere
			MD_AV=1;
			MES_Dist_AV ();
		}	
	}

	currentPosCmd = 0;

	MD_AV=0;
	MD_AR=0;
}

void main(){
	WDTCN = 0xDE;   // Devalidation du watchdog 
	WDTCN = 0xAD;

	OSCXCN =  0xef; //configure external oscillator
	delay_10us();
	while(!(OSCXCN&(0x80))){}

	OSCICN = OSCICN | 0x08; //utilise l'oscillateur externe
	
	init_1();
	while(1) 
	{
		// Gestion cmd
		
		mesure_distance(typeCmd,cmd);
		timerDelay();
			
		
	}
}
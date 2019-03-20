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


char currentPosCmd = 0; //iterateur sur la commande
int distance;

// Fonction d'initialisation des differents registres
void init_dist(){
	distance = -1; 
	//WDTCN = 0xDE;
	//WDTCN = 0xAD;
	
	XBR2 = XBR2 | 0x40;
	//P1MDOUT = P1MDOUT | 0x40;
	P3MDOUT = P3MDOUT | 0x1C; //Gestion des entrées/sorties
	MD_AV=0;
	MD_AR=0;
	ADC0CN = ADC0CN | 0x80;
	
	T4CON = 0x01; 
	
	EA = 1;
	EIE2 = EIE2 | 0x10; //Autorisation interruption 6 (externe)
	P3IF = P3IF | 0x04; //front montant
}
// Delay pour le trigger du télémètre
void delay_10us(){ //13us
	int i;
	for(i=0;i<22;i++){}
}

void timerDelay(){ //delai de 10ms
	int j;
	j = 0;
	for(j = 0; j < 25000; j++){
	}
}

int MES_Dist_AV (void){ //Mesure distance avant
	delay_10us();
	
	MD_AV = 0;
	timerDelay();
	return distance;
}
	
int MES_Dist_AR (void){ //Mesure distance arriere 
	
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
		T4CON = T4CON & 0xFB; //desactivation du timer4
		distance = (TH4*256 + TL4)/20; //conversion distance en cm
		if(distance == 0){
			distance = -1;
		}
		P3IF = P3IF | 0x04;
	}
	P3IF = P3IF & 0xBF;
}
int mesure_distance(char* cmd, char* param1){
	if(strcmp(cmd,"MOU")==0){
			if(strcmp(param1, "B") == 0){ //'Back' pour l'arriere
				MD_AR=1;
				MES_Dist_AR ();
			} 
			else {
			if(strcmp(param1, 'F') == 0){ //'Front' pour l'arriere
				MD_AV=1;
				MES_Dist_AV ();
			} 
			else {
				return -1;
			}
		}
	}
	while(distance == 0){
	}
	
	MD_AV=0;
	MD_AR=0;
	
	return distance;
}
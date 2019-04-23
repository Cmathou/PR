#include "c8051F020.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
//__________________________________________regarder p251 pour prochaine fois, pb d'overflow trop long______________________

/*
float Intensite = 90;
int Duree1 = 5; //durée en ms 2 ? 
int Duree2 = 7; //9 sec ? 
int Cycles=3;
*/


int cpt=0;	
int d=4;


// a enlver mais modifier dans pri,cipal
void en_pin_output(void){
	  
    XBR2 |= 0x40;
		P0MDOUT |= 0x0A;
		XBR0 |= 0x10;	//attribution du CEX0 et 1 au ports pin  __PWM__
}



void PCA_PMW(){  //__PWM__
	PCA0CN |= 0x00; 
	PCA0MD |= 0x01; //enable interrupt et sysclk/12
	PCA0CPM1 |= 0x42; //mode PMW __ SFR address = 0xDB
	EIE1 |= 0x08; //enable interrupt PCA
}

void initLaser() {
	PCA_PMW();
	en_pin_output();
}

void receptionCommande() interrupt 9 { //interrupt PCA
	CF=0;
	//CCF1=0;
	if (cpt<d){   //1 ms
		cpt= cpt + 1;
	}
	else {
		CR=0;
		cpt=0;
	}
}



void LumiereDegra(char* Intensite, char* Duree1,char* Duree2, char* Cycles){
int CycleActuel=0;
int OffActuel=0;
int OnActuel=0;
float RapCycl = atoi(Intensite) * 0.01;
int DureeON = atoi(Duree1) * 5;
int DureeOFF = atoi(Duree2) * 5;



		while (CycleActuel < atoi(Cycles))
			{	
			PCA0CPH1 = -(RapCycl*256)+256; //PCA0CPH1 SFR adress = 0xFB     1cycle = 0.1385 ms 				
			while (OnActuel<DureeON)
			{
				CR=1;
				while (CR==1){
				}				
				OnActuel=OnActuel+1;
			}
			PCA0CPH1 = 255;	
			while (OffActuel<DureeOFF)
			{
				CR=1;
				while (CR==1){
				}	
				OffActuel=OffActuel+1;
			}
			CycleActuel=CycleActuel+1;
			OnActuel=0;
			OffActuel=0;
		}
}

#include "c8051F020.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

int d=10; 
int cpt=0;	
int cde_Led=0;

void receptionCommande() interrupt 5 { //interruption du timer 2
	TF2=0;
	if (cpt<d){   //1 ms
		cpt= cpt + 1;
	}
	else {
		TR2=0;
		cpt=0;
	}
}

void en_TIMER_2(void){
	CKCON|=0x20; //timer 2 uses the system clk (22Mhz => 4.5*10^-5 ms)
	T2CON&=0x9C; //mode autoreaload
	TF2=0;
	TR2=0;
	RCAP2L=0x5C;
	RCAP2H=0xF9;
	ET2=1; //enable interrupt timer 2
	
}

void LumiereDegra(char* Intensite,char* Duree1,char* Duree2,char* Cycles){
int a=0;
int b=0;
int c=0;
float RapCyclique = atoi(Intensite)*0.1;
int DureeON = atoi(Duree1);
int DureeOFF = atoi(Duree2);
int nbCycle = atoi(Cycles);


		while (a<nbCycle){
			while (b<DureeON){
				cde_Led = 1;
				cpt = d*(1-RapCyclique);
				TR2=1; //lancement du timer	
				while (TR2==1){
				}
				cde_Led=0;
				cpt = d*(RapCyclique);
				TR2=1;
				while (TR2==1){
				}
				b=b+1;
			}
			while (c<DureeOFF){
				cde_Led = 0;
				cpt=0;
				TR2=1; //lancement du timer
				while (TR2==1){
				}	
				c=c+1;
			}
			a=a+1;
			b=0;
			c=0;
		}
}

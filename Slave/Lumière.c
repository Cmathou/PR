#include "c8051F020.h"


int Init=1;  //a mettre plus tard en chaine de chara: char init[4];
float RapCyclique = 0.4;
int DureeON = 10000; //durée en ms
int DureeOFF = 5000;
int cpt=0;	
int cde_Led=0;
int nbCycle=3;


//----------------------------------délai de n ms----------------------------------------------------------------------
void delay(int n){
	int j;
	int i;
	for (i=0;i<n;i++){
		for (j=0;j<1200;j++){
		}	
	}
}

void disable_watchdog(void){
   WDTCN = 0xDE;   // Dévalidation du watchdog 
   WDTCN = 0xAD;
}

void en_ext_sysclock(void){
    
    OSCXCN = 0x67; //Activation oscillator mode 
    delay(100); // temps d'attente pour etablir l'oscillateur externe
    OSCICN= 0x08; // Extinction de l'oscillateur interne
    
}


void receptionCommande() interrupt 5 { //interruption du timer 2
	TF2=0;
	if (cpt<22000){   //1 ms
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
	ET2=1; //enable interrupt timer 2
}

void LumiereDegra(int Init,float RapCyclique,int DureeON,int DureeOFF,int nbCycle){
int n=0;
int i=0;
int h=0;


	if (Init==0){   //plus tard faire init == "off"
	  cde_Led=0;    //cde_Led=0
	}
	if (Init==1){
		while (n<nbCycle){
			while (i<DureeON){
				cde_Led = 1;
				cpt = cpt + 22000*(1-RapCyclique);
				TR2=1; //lancement du timer	
				while (TR2==1){
				}
				cde_Led=0;
				cpt = cpt + 22000*(RapCyclique);
				TR2=1;
				while (TR2==1){
				}
				i=i+1;
			}
			while (h<DureeOFF){
				cde_Led = 0;
				cpt=0;
				TR2=1; //lancement du timer
				while (TR2==1){
				}	
			}
			n=n+1;
		}
	}
}

void main(void){
	disable_watchdog();
	en_ext_sysclock();
	en_TIMER_2();
	EA=1;
	LumiereDegra(Init,RapCyclique,DureeON,DureeOFF,nbCycle);

	while(1){
	}
}

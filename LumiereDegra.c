#include "c8051F020.h"


int Init=0;  //a mettre plus tard en chaine de chara: char init[4];
float RapCyclique = 0.4;
int Duree = 10;
int cpt=0;	
int cde_Led=0;


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
	TR2=0;
	if (cpt<22000){   //1 ms
		cpt= cpt + 1;
	}
}

void en_TIMER_2(void){
	CKCON|=0x20; //timer 2 uses the system clk (22Mhz => 4.5*10^-5 ms)
	T2CON&=0x9E; //mode autoreaload
	TF2=0;
	TR2=0;
}

void LumiereDegra(int Init,float RapCyclique,int Duree){
int n=0;


	if (Init==1){   //plus tard faire init == "off"
	  cde_Led=0;    //cde_Led=0
	}
	if (Init==0){
		while (n<Duree){
			cde_Led = 1;
			cpt = cpt + (22000*(1-RapCyclique));
			TR2=1; //lancement du timer	
			cde_Led=0;
			cpt = cpt + (22000*(RapCyclique));
			TR2=1;
			n=n+1;
		}
	}
}

void main(void){
	disable_watchdog();
	en_ext_sysclock();
	en_TIMER_2();
	EA=1;
	LumiereDegra(Init,RapCyclique,Duree);

	while(1){
	}
}

#include "c8051F020.h"

sbit LED = P1^6;
char lettre;
unsigned int n,p;

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
void main (void) 
{
	  
		WDTCN = 0xDE; 					// Desactivatin du watchdog 
		WDTCN = 0xAD;
	
		EIE1 |= 0x01;				 	// Interruptions
		EA = 1;
	
		XBR2 |= 0x40;				// Active Crossbar
		XBR0 |= 0x02;			 //Route les 4 ports de la SPI et les 2 de l'UART0;
	
	  
		OSCXCN = 0x67;		//Configuration SYSCLK à 22MHz
		for (n=0;n<2;n++){
			for (p=0;p<2500;p++){
			}
		}
		OSCICN |= 0x08;

	
		P0MDOUT |= 0xFF;	// Activation des ports P0.x
		P1MDOUT |= 0xFF;	// Activation des ports P0.x

		SPI0CFG = 0x87;	//Configure CLK SPI (actif front montant) + transmission des 8 bits
		
		SPI0CKR = 0x48; //Definit frequence CLK sur celle de SYSCLK.
		
		SPI0CN |= 0x01;	//active la SPI
		LED = 0;
		while(1){
		}
}

void test() interrupt 6 {
	SPIF = 0;
	if (LED == 1){
		LED =0;
	}
	else {
		LED = 1;
	}
}


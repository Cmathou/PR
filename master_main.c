#include "c8051F020.h"

sbit Slave_select = P1^0;
sbit LED = P1^6;
unsigned int n,p,k;

//------------------------------------------------------------------------------------


void delay(int n){
	int j;
	int i;
	for (i=0;i<n;i++){
		for (j=0;j<198;j++){
		}	
	}
}
void send_char(char a){
		Slave_select = 0;
		SPI0DAT = a;
		while(TXBSY == 1) {}
		Slave_select = 1;
	}
// MAIN Routine
//------------------------------------------------------------------------------------
void main (void) 
{
    WDTCN = 0xDE;  				// D?validation du watchdog 
    WDTCN = 0xAD;
	
	
	EIE1 |= 0x01;				//Interruptions
	EA = 1;
	
	
	XBR2 |= 0x40;		// Active Crossbar
	XBR0 |= 0x02;	 //Route les 4 ports de la SPI et les 2 de l'UART0;
	
	
	OSCXCN = 0x67;		//Configuration SYSCLK à 22MHz
	for (n=0;n<2;n++){
		for (p=0;p<2500;p++){
		}
	}
	OSCICN |= 0x08;	
	
	
	P0MDOUT |= 0xFF;			// Activation des ports P0.x
	P1MDOUT |= 0xFF;
	
	SPI0CFG = 0x87;//Configure CLK SPI (actif front montant) + transmission des 8 bits
	
	SPI0CN |= 0x02;	//active mode master
	
	SPI0CKR = 0x48; //D?finit fr?quence CLK sur celle de SYSCLK.
	
	Slave_select = 0;
	
	SPI0CN |= 0x01;	//active la SPI
	LED = 0;
	while(1){
		send_char('b');
		delay(5000);
	}  	
}

void test()interrupt 6 {
	SPIF = 0;
	if (LED == 1){
		LED =0;
	}
	else {
		LED = 1;
	}
}
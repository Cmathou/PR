#include "c8051F020.h"
#include <string.h>
#include "ringB/UART0_RingBuffer_lib.h"

sbit Slave_select = P2^0;

//------------------------------------------------------------------------------------

void spichar(char a){
		Slave_select = 0;
		SPI0DAT = a;
		while(TXBSY == 1) {}
		Slave_select = 1;
	}

void spistring(char* msg) {
	char i;
	for (i = 0; i < strlen(msg); i++) {
		spichar(msg[i]);
	}
}
	
void initSPI() {
	EIE1 |= 0x01;				//Interruptions
	
	XBR2 |= 0x40;		// Active Crossbar
	XBR0 |= 0x02;	 //Route les 4 ports de la SPI;
	
	P0MDOUT |= 0xF0;			// Activation des ports P0.x
	P2MDOUT |= 0x01;
	
	SPI0CFG = 0x87;	//Configure CLK SPI (actif front montant) + transmission des 8 bits
	
	SPI0CN |= 0x02;	//active mode master
	
	SPI0CKR = 0x48; //D?finit fr?quence CLK sur celle de SYSCLK.
	
	Slave_select = 0;
	
	SPI0CN |= 0x01;	//active la SPI
}
	
void spiint()interrupt 6 {
	SPIF = 0;
}
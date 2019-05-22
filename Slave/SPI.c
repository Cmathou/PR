#include "c8051F020.h"
#include <string.h>
#include "SPI.h"
#include "Slave/ringB/UART0_RingBuffer_lib.h"


char spir[32] = "";
char spiflag = 0;

void init_SPI(void) 
{	  
		EIE1 |= 0x01;				 	// Interruptions
	
		XBR2 |= 0x40;				// Active Crossbar
		XBR0 |= 0x02;			 //Route les 4 ports de la SPI 
		 
		P0MDOUT |= 0xF0;	// Activation des ports P0.x
	
		SPI0CFG = 0x87;	//Configure CLK SPI (actif front montant) + transmission des 8 bits
		SPI0CKR = 0x48; //Definit frequence CLK sur celle de SYSCLK.
		SPI0CN |= 0x01;	//active la SPI
}

void spiint() interrupt 6 {
	char c[2] = "";
	SPIF = 0;
	c[0] = SPI0DAT;
	if (c[0] == '\n') {
		spiflag = 1;
	} else {
		strcat(spir, c);
	}
}
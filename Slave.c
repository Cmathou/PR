#define uchar	unsigned char


sbit SCK	= P0^2;
sbit MISO	= P0^3; 
sbit MOSI	= P0^4;
sbit NSS	= P1^0;


uchar SPI_Slave(void){
		SPI0CFG |= 0x07;											//On configure le nb de bit  envoyés ainsi que la phase et polarité
		SPI0CFG &= 0x3F;											
		SPI0CN |= 0x01; 											//On met en mode master et on active SPI
		SPI0CKR = 0x6D;
}	

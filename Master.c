#define uchar	unsigned char


sbit SCK	= P0^2;
sbit MISO	= P0^3; 
sbit MOSI	= P0^4;
sbit select_slave	= P1^0;


void SPI_Master(void){
		select_slave = 1;
		SPI0CFG |= 0x07;											//On configure le nb de bit  envoyés ainsi que la phase et polarité
		SPI0CFG &= 0xC0;											
		SPI0CN |= 0x03; 											//On met en mode master et on active SPI
		SPI0CKR = 0x6D;
		
}

void send_char(char c){
	select_slave = 0;
	SPI0DAT = c;
	while(TXBSY==1){}
	select_slave = 1;	
}
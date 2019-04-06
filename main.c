void main(void){
int i;															 // delay counter
WDTCN = 0xde;                       // disable watchdog timer
WDTCN = 0xad;
OSCXCN = 0x67;                      // start external oscillator with                                 // 22.1184MHz crystal
for (i=0; i < 256; i++) {}          // wait for oscillator to start
while (!(OSCXCN & 0x80)) {}         // Wait for crystal osc. to settle
OSCICN = 0x88;                      // select external oscillator as SYSCLK source and enable missing clock detector
XBR0    |= 0x02;                    // Enable SPI
XBR2    |= 0x40;                    // Enable crossbar and weak pull-ups
P0MDOUT |= 0x04;                    // enable SCK as a push-pull output
P1MDOUT |= 0x01;                    // enable NSS as a push-pull output

	while(1){
	
	send_char(b);
	
	}
}
#include "c8051F020.h"

//cmd servomotors : CS H/V [angle]

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int j = 0;
char currentPosCmd = 0; //iterateur sur la commande
char cmdRecue = 0; //bool indiquant qu'il y a une commande à traiter
char cmd[20] = {0}; //chaine complete de commande
char typeCmd[5] = {0}; //premières lettres de la commande; indiquant son type
char periodServoHoriz = 500;
char timeHighServoHoriz = 150;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void timerDelay(){ //delai de 10us
	j = 0;
	for(j = 0; j < 25; j++){
	}
}

void enable_int () {
	EA = 1; // General interrupt enable
	ET2 = 0x01; // TIMER2-OVFLW
	ES0 = 1; // UART interrupt enable
	return;
}


void interrupt_UART0() interrupt 4
{
	if(TI0){
		TI0 = 0;
	}
	if(RI0){
		RI0 = 0;
		cmd[currentPosCmd] = SBUF0;
		if(cmd[currentPosCmd] == '\r'){ //!\\ verifier dernier caractère envoyé par Putty
			cmdRecue = 1;
			currentPosCmd = 0;
		}
	}
}

void configTimer2(){
	RCLK0 = 1;//UART0
	TCLK0 = 1;//UART0
	CPRL2 = 0;
	CKCON |= 0x20;
	RCAP2H = 0xFF;
	RCAP2L = 0xB8;
	TR2 = 1; // Activation du timer 2
}


void configUART0(){
	P0MDOUT = 0xFF;
	PCON = 0x00;
	SCON0 = 0x70;
	XBR0 |= 0x04;
}


void init(){
	configTimer2();
	enable_int();
	configUART0();
	P3MDOUT = P3MDOUT | 0x10;
}

char* comprehensionCmd(char* cmd){
	while(cmd[currentPosCmd] != ' '){
		typeCmd[currentPosCmd] = cmd[currentPosCmd];
		currentPosCmd++;
	}
	currentPosCmd = 0;
	return typeCmd;
}

void ServoHorizontal(typeCmd, cmd){
	char* consigne;
	char* currentPosConsigne;
	if(typeCmd == "CS"){
		while(cmd[currentPosCmd] != ' '){ //relecture CS
			currentPosCmd++;
		}
		currentPosCmd++;
		if(cmd[currentPosCmd] == 'H'){ // 'H' pour le servo horizontal, 'V' pour le vertical
			while(cmd[currentPosCmd] != ' '){
				currentPosCmd++;
				consigne[currentPosCmd] = cmd[currentPosCmd];
			}
			timeHighServoHoriz = 
		}
		currentPosCmd = 0;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main(){
	WDTCN = 0xDE;   // Devalidation du watchdog 
	WDTCN = 0xAD;

	OSCXCN =  0xef; //configure external oscillator
	timerDelay();
	while(!(OSCXCN&(0x80))){}

	OSCICN = OSCICN | 0x08; //utilise l'oscillateur externe
	
	init();

	while(1) 
	{
		// Gestion cmd
		if(cmdRecue){
			cmdRecue = 0;
			comprehensionCmd(cmd);
			
			
		}
	}
}





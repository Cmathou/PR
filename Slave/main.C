#include <stdio.h>
#include <string.h>
#include "c8051F020.h"
#include "command.h"
#include "COM_UART.h"
#include "timers.h"
#include "servo_V.h"
#include "laser.h"
#include "ringB/UART0_RingBuffer_lib.h"
#include "ringB/UART1_RingBuffer_lib.h"
#include "SPI.h"

static char cmd[32] = "\0";

void putty() {
	char c[2] = "";
	while ((c[0] = serInchar()) != 0) {
			serOutchar(c[0]);
			if (c[0] == '\r') {
				serOutchar('\n');
				process(cmd);
				cmd[0] = '\0';
			}
			else {
				strcat(cmd, c);
			}
		}
}

void spicmd() {
	if (spiflag == 1) {
		spiflag = 0;
		process(spir);
		spir[0] = '\0';
	}
}

void callback() {
	timeLaser();
}

void main(void) {
	
	WDTCN = 0xde;                       // disable watchdog timer
	WDTCN = 0xad;
	SYSCLK_Init ();                     // initialize oscillator
	PORT_Init ();                       // initialize crossbar and GPIO
	cfg_Clock_UART();
	cfg_UART0_mode1();
	cfg_UART1_mode1();
	init_Serial_Buffer();
	init_Serial_Buffer1();
	
	EA = 1;
	
	//init
	timer_3();
	initServoV();
	init_SPI();
	initLaser();
	
	
	while (1) {
		//putty();
		spicmd();
		callback();
	}
}
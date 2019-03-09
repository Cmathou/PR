#include "c8051F020.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use an 22.1184MHz crystal
// as its clock source.
//
void SYSCLK_Init(void) {
   int i;                              // delay counter

   OSCXCN = 0x67;                      // start external oscillator with
                                       // 22.1184MHz crystal
   for (i=0; i < 256; i++) ;           // wait for oscillator to start
   while (!(OSCXCN & 0x80)) ;          // Wait for crystal osc. to settle
   OSCICN = 0x88;                      // select external oscillator as SYSCLK
                                       // source and enable missing clock
                                       // detector
}
//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports
//
void PORT_Init(void) {
   XBR0    |= 0x04;                    // Enable UART0
   XBR2    |= 0x04;                    // Enable UART1
   XBR2    |= 0x40;                    // Enable crossbar and weak pull-ups
   P0MDOUT |= 0x05;                    // enable TX0 and TX1 as a push-pull output
}
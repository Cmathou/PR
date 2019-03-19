#include "c8051F020.h"

void timer_3(){
	TMR3CN |= 0x02;
	TMR3RLL = 0x66;
	TMR3RLH = 0x56;
	TMR3L = 0x66;
	TMR3H = 0x56;
}
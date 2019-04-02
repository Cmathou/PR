#include "c8051F020.h"
#include "timers.h"

static timeType time = 0;

void timer_3(){
	TMR3CN |= 0x02;
	TMR3RLL = 0x66;
	TMR3RLH = 0x56;
	TMR3L = 0x66;
	TMR3H = 0x56;
}

timeType getTime(timeType cmd) {
	return time + cmd;
}

char timePass(timeType cmd) {
	return cmd <= time;
}
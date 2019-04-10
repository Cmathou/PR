#include "c8051F020.h"
#include "timers.h"

static timeType time = 0;

void timer_3() {
    TMR3CN |= 0x02;  //use system clock
    TMR3RLL = 0x66;  // set for 1ms timer
    TMR3RLH = 0x56;
    TMR3L = 0x66;
    TMR3H = 0x56;
    EIE2 |= 0x01;
    TMR3CN |= 0x04;
}

void timer3OF() interrupt 14 {
    time++;
    TMR3CN &= 0x7F;
}

timeType getTime(timeType cmd) {
    return time + cmd;
}

char timePass(timeType cmd) {
    if (cmd == 0)
        return 0;
    if (cmd <= time)
        return 1;
    else
        return 0;
}
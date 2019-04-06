#include <stdlib.h>
#include <string.h>
#include "c8051F020.h"

//cmd servomotors : CS H/V [angle]
//freq oscillator = 22.1184 MHz
//1 us ~= 22 tick

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int timeHighServoVerti = 1050;
unsigned int valHigh = 0;

sbit outServoV = P3 ^ 7;

int dist = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void timerDelay() {  //delai de 10ms
    int j;
    for (j = 0; j < 25000; j++) {
    }
}

void enable_int() {
    EA = 1;  // General interrupt enable
    EIE1 |= 0x08;
}

void configPCA0() {
    PCA0MD = 0x09;
    PCA0CPM0 = 0x49;
    CR = 1;
}

void initServoV() {
    configPCA0();
    enable_int();
    P3MDOUT = P3MDOUT | 0x80;  //Passe le pin 3.7 en push-pull
    XBR2 = XBR2 | 0x40;
}

void setServoVertical(char* cmd, char* param1, char* param2) {
    if (strcmp(param1, 'V')) {                          // 'H' pour le servo horizontal, 'V' pour le vertical
        timeHighServoVerti = 1500 + atoi(param2) * 10;  //consigne = +/- 90 => timeHigh = [600; 2400] us

        if (timeHighServoVerti < 600) {
            timeHighServoVerti = 600;
        }
        if (timeHighServoVerti > 2400) {
            timeHighServoVerti = 2400;
        }

        valHigh = timeHighServoVerti * 22;

        PCA0CPL0 = valHigh & 0xFF;
        PCA0CPH0 = (valHigh >> 8) & 0xFF;

        return 0;
    } else {
        return 1;
    }
}

void ServoVertical() interrupt 9 {
    if (CCF0 == 1) {
        CCF0 = 0;
        outServoV = 0;
    }
    if (CF == 1) {
        CF = 0;
        outServoV = 1;
    }
}

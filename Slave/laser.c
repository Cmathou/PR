#include "c8051F020.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "timers.h"

static timeType time = 0;
static int Intense;
static int CyclesT;
static int CycleActuel;
static int DureeON;
static int DureeOFF;

void switchState() {
	if (PCA0CPH1 != 255) {
		PCA0CPL1 = 255;
		PCA0CPH1 = 255;
		time = getTime(DureeOFF);
	} else {
		CycleActuel++;
		if (CycleActuel < CyclesT) {
			PCA0CPL1 = Intense & 0xFF;
			PCA0CPH1 = Intense >> 8;
			time = getTime(DureeON);
		}
	}
}

void timeLaser() {
    if (timePass(time)) {
			time = 0;
			switchState();
    }
}

// a enlever mais modifier dans principal
void en_pin_output(void){
    XBR2 |= 0x40;
		P1MDOUT |= 0x02;
		XBR0 |= 0x10;	//attribution du CEX0 et 1 au ports pin  __PWM__
}



void PCA_PMW(){  //__PWM__
	PCA0CN |= 0x00; 
	PCA0MD |= 0x01; //enable interrupt et sysclk/12
	PCA0CPM1 |= 0xC2; //mode PMW __ SFR address = 0xDB
	EIE1 |= 0x08; //enable interrupt PCA
}

void initLaser() {
	PCA_PMW();
	en_pin_output();
	CR = 1;
	PCA0CPL1 = 255;
	PCA0CPH1 = 255;
}

void LumiereDegra(char* Intensite, char* Duree1, char* Duree2, char* Cycles){
	float RapCycl = atoi(Intensite) * 0.01;
	Intense = -(RapCycl*65536) + 65536;
	CyclesT = atoi(Cycles);
	CycleActuel = 0;
	DureeON = atoi(Duree1);
	DureeOFF = atoi(Duree2);
	
	if (Cycles > 0) {
		PCA0CPL1 = Intense & 0xFF;
		PCA0CPH1 = Intense >> 8;
		time = getTime(DureeON);
	}
}

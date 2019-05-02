//------------------------------------------------------------------------------------
// Detection_Obstacles_1.c
//------------------------------------------------------------------------------------
// DATE: 27/02/19
//
// Target: C8051F02x
// Tool chain: KEIL Microvision 4
//
//  NOMS: Prummel
//
//------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "c8051F020.h"

//sbit LED = P1 ^ 6;
// MD = P3 ^ 2 ;
sbit MD_AV = P3 ^ 3;
sbit MD_AR = P3 ^ 4;

static int xdata distance;

static char xdata mesure_flag = 0;

// Fonction d'initialisation des differents registres
void initObs() {
    XBR2 |= 0x40;
    P3MDOUT |= 0x1C;  //Gestion des entrées/sorties
    MD_AV = 0;
    MD_AR = 0;

    ADC0CN |= 0x80;

    T4CON = 0x01;

    EA = 1;
    EIE2 |= 0x10;  //Autorisation interruption 6 (externe)
    P3IF |= 0x04;  //front montant
    P3IF &= 0xBF;

    mesure_flag = 0;
}

// Delay pour le trigger du télémétre
void delay_10us() {  //13us
    int i;
    for (i = 0; i < 22; i++) {
    }
}

void MES_Dist_AV(void) {  //Mesure distance avant
    MD_AV = 1;
    delay_10us();
    MD_AV = 0;
}

void MES_Dist_AR(void) {  //Mesure distance arriere
    MD_AR = 1;
    delay_10us();
    MD_AR = 0;
}
void mesure() interrupt 18 {
    if ((P3IF & 0x04) == 0x04) {  //P3IF is rising edge
        TH4 = 0;
        TL4 = 0;
        T4CON |= 0x04;  //activation du timer4
        P3IF &= 0xFB;   //set on falling edge
    } else {
        T4CON &= 0xFB;                       //desactivation du timer4
        distance = (TH4 * 256 + TL4) / 120;   //conversion distance en cm
        P3IF |= 0x04;                        //set rising edge
        mesure_flag = 1;
    }
    P3IF &= 0xBF;
}

void mesure_distance(char *typeCmd, char *cmd) {
	if (strcmp(cmd, "B") == 0) {  //'Back' pour l'arriere
		MES_Dist_AR();
	}
	else {
		if (strcmp(cmd, "F") == 0) {  //'Front' pour l'arriere
			MES_Dist_AV();
		} else {
			distance = -1;
			mesure_flag = 1;
		}
	}
}

char* MOU(char *typeCmd, char *cmd) {
    char ret[] = 0;
		mesure_flag = 0;
    mesure_distance(typeCmd, cmd);
    while (!mesure_flag) {}
    mesure_flag = 0;
    sprintf(ret, "%d", distance);
    return ret;
}
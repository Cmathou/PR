#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c8051F020.h"
#include "SPI.h"
#include "command.h"
#include "obstacle.h"
#include "ringB/UART0_RingBuffer_lib.h"
#include "ringB/UART1_RingBuffer_lib.h"
#include "serilizer.h"
#include "servo_H.h"

static char D_nbr  = 0;

int process(char* cmd_str) {
    char retour[8];
    char cmd[4] = "";
    char param1[7] = "";
    char param2[7] = "";
    char param3[7] = "";
    char param4[7] = "";
    sscanf(cmd_str, "%s %s %s %s %s", cmd, param1, param2, param3, param4);
		
    if (strcmp(cmd, "Q") == 0) {  //arret urgence
        spistring(cmd);
        spichar('\n');
        RSTSRC |= 0x10;
        return 0;
    }

    if (strcmp(cmd, "D") == 0) {  //debut epreuve
        D(param1);
        return 0;
    }

    if (D_nbr != 0) {

        if (strcmp(cmd, "L") == 0) {  //lumiere
            spistring(cmd_str);
            spichar('\n');
            valid();
            return 0;
        }
    
        if (strcmp(cmd, "LS") == 0) {  //lumiere
            spistring(cmd);
            spichar('\n');
            valid();
            return 0;
        }

        if (strcmp(cmd, "A") == 0) {  //avance
            AB(param1, '0');
            return 0;
        }

        if (strcmp(cmd, "B") == 0) {  //recule
            AB(param1, '1');
            return 0;
        }

        if (strcmp(cmd, "TV") == 0) {  //vittesse par default
            TV(param1);
            return 0;
        }

        if (strcmp(cmd, "S") == 0) {  //stop les moteurs
            S();
            return 0;
        }

        if (strcmp(cmd, "RD") == 0) {  //rotation de 90° droite
            R(0, 0);
            return 0;
        }

        if (strcmp(cmd, "RG") == 0) {  //rotation de 90° gauche
            R(1, 0);
            return 0;
        }

        if (strcmp(cmd, "RC") == 0) {  //rotation de 180°
            if (strcmp(param1, "D") == 0) {
                R(0, 1);
            } else {
                if (strcmp(param1, "G") == 0) {
                    R(1, 1);
                } else {
                    invalid();
                }
            }
            return 0;
        }

        if (strcmp(cmd, "CS") == 0) {  //servomoteur
					if (strcmp(param1, "H") == 0) {
            if (ServoHorizontal(cmd, param1, param2) == 1) {
                valid();
            } else {
                invalid();
            }
					} else {
						spistring(cmd_str);
						spichar('\n');
						valid();
					}
					return 0;
        }

        if (strcmp(cmd, "MOU") == 0) {  //mesure distance
            strcpy(retour, MOU(cmd, param1));
            if (retour != -1) {
                serOutstring("Distance : ");
                serOutstring(retour);
                serOutstring("\r\n");
                valid();
            } else {
                invalid();
            }
            return 0;
        }
    }

    if (strcmp(cmd, "E") == 0) {  //fin d'epreuve
        D_nbr = 0;
        valid();
        return 0;
    }

    invalid();
    return 1;
}

////////////////////////////////////////////////////////
//interrupt function
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//functions
////////////////////////////////////////////////////////

void valid() {
    serOutstring(">");
}

void invalid() {
    serOutstring("#");
}

void D(char* param) {
    int d = atoi(param);
    if (d > 0 && d < 9) {
        D_nbr = d;
        serOutstring("début de l'épreuve!\r\n");
        valid();
    } else {
        invalid();
    }
}
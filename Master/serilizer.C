#include "c8051F020.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timers.h"
#include "command.h"
#include "serilizer.h"
#include "ringB/UART0_RingBuffer_lib.h"
#include "ringB/UART1_RingBuffer_lib.h"

#define R90 430
#define rot 500.0/90
#define dist 2500.0/8.3

static timeType time = 0;
static int dS = 20;

static phase = 0;
static char params1[8];
static char params2[8];
static char params3[8];

void testValid() {
	valid();
	/*
	char c[2] = '\0';
	char answer[16] = "\0";
	while ((c[0] = serInchar1()) != 0) {
		if (c[0] == '>') {
			if (strcmp(answer, "\r\nACK\r\n") == 0) {
				valid();
			} else {
				invalid();
			}
			answer[0] = '\0';
		}
		else {
			strcat(answer, c);
		}
	}*/
}

void timeSerilizer() {
    if (timePass(time)) {
			time = 0;
			G(params1, params2, params3);
    }
}

void TV(char *param)
{
    int speed = atoi(param);
    if (5 <= speed && speed <= 100)
    {
        dS = speed;
        testValid();
    }
    else
    {
        invalid();
    }
}

void AB(char *param, char sign)
{
    int speed = atoi(param);
    char cmd[32] = "mogo 1:";
    if (strcmp(param, "") == 0)
    {
        sprintf(param, "%d", dS);
        if (sign == '1')
        {
            strcat(strcat(strcat(strcat(strcat(cmd, "mogo 1:-"), param), " 2:-"), param), "\r");
        }
        else
        {
            strcat(strcat(strcat(strcat(strcat(cmd, "mogo 1:"), param), " 2:"), param), "\r");
        }
        serOutstring1(cmd);
        testValid();
    }
    else
    {
        if (5 <= speed && speed <= 100)
        {
            if (sign == '1')
            {
                strcat(strcat(strcat(strcat(strcat(cmd, "mogo 1:-"), param), " 2:-"), param), "\r");
            }
            else
            {
                strcat(strcat(strcat(strcat(strcat(cmd, "mogo 1:"), param), " 2:"), param), "\r");
            }
            serOutstring1(cmd);
            testValid();
        }
        else
        {
            invalid();
        }
    }
}

void RA(char *param1, char *param2) {
	char outstr[64] = "digo 1:";
	int param = atoi(param2);
	char temp[8];
	param *= rot;
	sprintf(temp, "%d", param);
	if (strcmp(param1, "G") == 0) {
		strcat(strcat(strcat(strcat(outstr, temp), ":10 2:-"), temp), ":10\r");
		serOutstring1(outstr);
	} else {
		if (strcmp(param1, "D") == 0) {
			strcat(outstr, "-");
			strcat(strcat(strcat(strcat(outstr, temp), ":10 2:"), temp), ":10\r");
			serOutstring1(outstr);
		} else {
			invalid();
		}
	}
	testValid();
}

void G(char *param1, char *param2, char *param3) {
	char outstr[64] = "digo 1:";
	int x = atoi(param1);
	int y = atoi(param2);
	char param1t[8];
	char param2t[8];
	char param3t[8];
	strcpy(param1t, param1);
	strcpy(param2t, param2);
	strcpy(param3t, param3);
	if (phase == 0) {
		strcpy(params1, param1);
		strcpy(params2, param2);
		strcpy(params3, param3);
	}
	sprintf(param3t, "%d", 90 - atoi(param3t));
	x *= dist;
	y *= dist;
	sprintf(param1t, "%d", x);
	sprintf(param2t, "%d", y);
	if (phase == 0) {
			strcat(strcat(strcat(strcat(outstr, param2t), ":15 2:"), param2t), ":15\r");
			serOutstring1(outstr);
			outstr[0] = '\0';
			phase = 1;
			time = getTime(3000);
			testValid();
	} else {
		if (phase == 1) {
				RA("D", "90");
				phase = 2;
				time = getTime(1500);
		} else {
			if (phase == 2) {
					strcat(strcat(strcat(strcat(outstr, param1t), ":15 2:"), param1t), ":15\r");
					serOutstring1(outstr);
					phase = 3;
					time = getTime(3000);
					testValid();
			} else {
				if (phase == 3) {
						RA("G", param3t);
						phase = 0;
				}
			}
		}
	}
}

void S() {
    serOutstring1("stop\r");
    testValid();
}
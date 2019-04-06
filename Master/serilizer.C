#include "c8051F020.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timers.h"
#include "command.h"
#include "ringB/UART0_RingBuffer_lib.h"
#include "ringB/UART1_RingBuffer_lib.h"

#define R90 430

static timeType time = 0;
static int xdata dS = 20;

void timeSerilizer() {
    if (timePass(time)) {
			serOutstring1("stop\r");
			time = 0;	
    }
}

void TV(char *param)
{
    int xdata speed = atoi(param);
    if (5 <= speed && speed <= 100)
    {
        dS = speed;
        valid();
    }
    else
    {
        invalid();
    }
}

void R(char cas, int siz)
{
    if (cas == 0)
    {
        serOutstring1("mogo 1:-15 2:15\r");
    }
    else
    {
        serOutstring1("mogo 1:15 2:-15\r");
    }
    time = getTime((siz + 1) * R90);
    valid();
}

void AB(char *param, char sign)
{
    int xdata speed = atoi(param);
    char xdata cmd[32] = "mogo 1:";
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
        valid();
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
            valid();
        }
        else
        {
            invalid();
        }
    }
}

void S() {
    serOutstring1("stop\r");
    valid();
}
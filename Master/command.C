#include <stdio.h>
#include "command.h"

static void D_handler(char** values) {

}

static void E_handler(char** values) {

}

Handler handlers[MAX_CMD] = {
    {'D', D_handler},
    {'E', E_handler}
};
#include <stdio.h>
#include <string.h>
#include "command.h"

extern Handler handlers[];

static void process(Command* command) {
    for (int i = 0; i < MAX_CMD; i++) {
        Handler handler = handlers[i];
        if (strcmp(handler.cmd, command->cmd) == 0) {
            handler.handler(command->parameters);
        }
    }
}
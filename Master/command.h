#define MAX_CMD 2

typedef struct Handler {
    char *cmd[4];
    void* (*handler)(char**);
} Handler;

typedef struct Command {
    char *cmd[4];
    char parameters[3][64];
} Command;


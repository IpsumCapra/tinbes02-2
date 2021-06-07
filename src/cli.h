#include <constants.h>
#include <cli_commands.h>

#ifndef ARDUINOS_CLI_H
#define ARDUINOS_CLI_H

typedef char Parameters[][BUFSIZE];
typedef void (*Callback)(Parameters);

struct Command {
    Command(const char name[BUFSIZE], Callback callback, int parameters);
    char name[BUFSIZE];
    int parameters;
    Callback callback;
};

const Command commands[] = {
        {"store", &store, 2},
        {"retrieve", &retrieve,1},
        {"erase", &erase,1},
        {"files", &files, 0},
        {"freespace", &freespace, 0},
        {"run", &run, 1},
        {"list", &list, 0},
        {"suspend", &suspend, 1},
        {"resume", &resume, 1},
        {"kill", &kill,1},
        {"reformat", &reformat, 0}
};

bool handleCommand();
void processInput();

#endif //ARDUINOS_CLI_H

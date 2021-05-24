#include <constants.h>
#include <test.h>

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

static Command commands[] = {
        Command("store", store, 3),
        Command("retrieve", retrieve,1),
        Command("erase", erase,1),
        Command("files", files, 0),
        Command("freespace", freespace, 0),
        Command("run", run, 1),
        Command("list", list, 0),
        Command("suspend", suspend, 1),
        Command("resume", resume, 1),
        Command("kill", kill,1)
};

bool handleCommand();
void processInput();

#endif //ARDUINOS_CLI_H

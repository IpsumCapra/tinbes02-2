#include <constants.h>
#include <Arduino.h>

#ifndef ARDUINOS_MEMORY_H
#define ARDUINOS_MEMORY_H

struct stack {
    int sp;
    byte stack[STACKSIZE];
};

struct memEntry {
    char name;
    int type;
    int address;
    int process;
};

bool pushByte(byte b, stack &stack);

bool popByte(byte &b, stack &stack);

bool saveVar(byte name, int process);

#endif //ARDUINOS_MEMORY_H

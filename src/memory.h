#include <constants.h>
#include <Arduino.h>

#ifndef ARDUINOS_MEMORY_H
#define ARDUINOS_MEMORY_H

struct stack {
    int sp;
    byte stack[STACKSIZE];
};

bool pushByte(byte b, stack &stack);

bool popByte(byte &b, stack &stack);

#endif //ARDUINOS_MEMORY_H

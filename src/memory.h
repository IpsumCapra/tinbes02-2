#include <constants.h>
#include <Arduino.h>

#ifndef ARDUINOS_MEMORY_H
#define ARDUINOS_MEMORY_H

struct stack {
    int sp = 0;
    byte stack[STACKSIZE];
};

struct memEntry {
    char name;
    int type;
    int address;
    int process;
};

bool writeMemEntry(memEntry entry);
bool deleteMemEntry(int entry);

bool pushByte(byte b, stack &stack);
bool popByte(byte &b, stack &stack, bool peek);

int varEndAddress(memEntry entry);

bool findSpace(int size, int &address);

int varExists(char name, int process);

bool setVar(char name, int process, stack &stack);
bool getVar(char name, int process, stack &stack);
void wipeVars(int process);

bool pushInt(int i, stack &stack);
bool pushFloat(float f, stack &stack);

bool popInt(int &i, stack &stack, bool peek);
bool popFloat(float &f, stack &stack, bool peek);
bool popVal(float &f, stack &stack, bool peek);

bool popString(int &cp, stack &stack);

void memDump(stack stack);

#endif //ARDUINOS_MEMORY_H

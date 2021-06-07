#include <constants.h>
#include <Arduino.h>

#ifndef ARDUINOS_MEMORY_H
#define ARDUINOS_MEMORY_H

byte memory[RAMSIZE];

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

memEntry variables[MAXVARS];

bool writeMemEntry(memEntry entry);
bool deleteMemEntry(int entry);

bool pushByte(byte b, stack &stack);
bool popByte(byte &b, stack &stack);

int varEndAddress(memEntry entry);

bool findSpace(int size, int &address);

int varExists(char name, int process);

bool setVar(char name, int process, stack &stack);
bool getVar(char name, int process, stack &stack);
void wipeVars(int process);

#endif //ARDUINOS_MEMORY_H

#include <constants.h>
#include <Arduino.h>
#include <memory.h>

#ifndef ARDUINOS_PROCESSES_H
#define ARDUINOS_PROCESSES_H

struct processEntry {
    char name[NAMESIZE];
    byte state;
    int pc;
    int fp;
    int sp;
    int id;
    stack stack;
};

int retrieveProcess(int id);
bool runProcess(char name[]);
String printState(byte state);
bool setState(int id, byte state);
bool killProcess(int id);
void listProcesses();



#endif //ARDUINOS_PROCESSES_H

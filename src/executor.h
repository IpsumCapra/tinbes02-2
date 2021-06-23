#include <processes.h>

#ifndef ARDUINOS_EXECUTOR_H
#define ARDUINOS_EXECUTOR_H

void pushVal(processEntry &proc);
void pushString(processEntry &proc);

void print(processEntry &proc, bool newLine);

void unaryOperation(processEntry &proc);
void binaryOperation(processEntry &proc);

void readFromFile(processEntry &proc);

void execute(processEntry &proc);

#endif //ARDUINOS_EXECUTOR_H

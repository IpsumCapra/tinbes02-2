#include <Arduino.h>
#include <constants.h>

#ifndef ARDUINOS_TEST_H
#define ARDUINOS_TEST_H

typedef char Parameters[][BUFSIZE];

void store(Parameters);
void retrieve(Parameters);
void erase(Parameters);
void files(Parameters);
void freespace(Parameters);
void run(Parameters);
void list(Parameters);
void suspend(Parameters);
void resume(Parameters);
void kill(Parameters);
void reformat(Parameters);

#endif //ARDUINOS_TEST_H

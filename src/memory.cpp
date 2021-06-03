#include <memory.h>
#include <EEPROM.h>
#include <Arduino.h>

int noOfVars = 0;



bool pushByte(byte b, stack &stack) {
    int &sp = stack.sp;
    if (sp < STACKSIZE - 1) {
        stack.stack[sp++] = b;
        return true;
    }
    return false;
}

bool popByte(byte &b, stack &stack) {
    int &sp = stack.sp;
    if (sp > 0) {
        b = stack.stack[--sp];
        return true;
    }
    return false;
}

bool saveVar(byte name, int process) {
    return false;
}
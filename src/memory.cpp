#include "memory.h"

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
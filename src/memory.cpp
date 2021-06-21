#include <memory.h>
#include <Arduino.h>

int noOfVars = 0;
memEntry variables[MAXVARS];
byte memory[RAMSIZE];

bool writeMemEntry(memEntry entry) {
    if (noOfVars == MAXVARS) {
        return false;
    }
    variables[noOfVars++] = entry;
    return true;
}

bool deleteMemEntry(int entry) {
    if (noOfVars == 0) {
        return false;
    }

    if (noOfVars == 1) {
        noOfVars--;
        return true;
    }

    noOfVars--;
    for (; entry < noOfVars; entry++) {
        variables[entry] = variables[entry + 1];
    }
    return true;
}

bool pushByte(byte b, stack &stack) {
    int &sp = stack.sp;
    if (sp < STACKSIZE - 1) {
        stack.stack[sp++] = b;
        return true;
    }
    return false;
}

bool popByte(byte &b, stack &stack, bool peek) {
    int &sp = stack.sp;
    if (sp > 0) {
        b = stack.stack[peek ? sp-1 : --sp];
        return true;
    }
    return false;
}

int varEndAddress(memEntry entry) {
    int varType = entry.type;
    int pointer = entry.address;
    if (varType != STRING) {
        while (memory[pointer++] != 0) {}
        return pointer;
    } else {
        return pointer + varType;
    }
}

bool findSpace(int size, int &address) {
    if (size > RAMSIZE) {
        return false;
    }

    if (noOfVars == 0) {
        address = 0;
        return true;
    }

    if (noOfVars == 1) {
        int temp = varEndAddress(variables[0]);
        if (size < RAMSIZE - temp) {
            address = temp;
            return true;
        } else {
            return false;
        }
    }

    for (int i = 0; i < noOfVars; i++) {
        memEntry entry = variables[i];
        int endAddr = varEndAddress(entry);
        if (i < noOfVars - 1) {
            memEntry next = variables[i+1];
            if (next.address - endAddr >= size) {
                address = endAddr;
                return true;
            }
        } else {
            if (RAMSIZE - endAddr >= size) {
                address = endAddr;
                return true;
            }
        }
    }
    return false;
}

int varExists(char name, int process) {
    if (noOfVars == 0) {
        return -1;
    }
    for (int i = 0; i < noOfVars; i++) {
        if (variables[i].name == name && variables[i].process == process) {
            return i;
        }
    }
    return -1;
}

bool setVar(char name, int process, stack &stack) {
    // Delete if exists.
    int exists = varExists(name, process);
    if (exists != -1) {
        deleteMemEntry(exists);
    }

    // Too many vars?
    if (noOfVars == MAXVARS) {
        Serial.println(F("Max amount reached."));
        return false;
    }

    // Get variable type from stack.
    byte type;
    if (!popByte(type, stack, false)) {
        Serial.println(F("Stack not valid"));
        return false;
    }

    // Determine variable size.
    byte size;
    if (type != STRING) {
        size = type;
    } else {
        if (!popByte(size, stack, false)) {
            Serial.println(F("No string size found."));
            return false;
        }
    }

    // Try to allocate space.
    int address;
    if(!findSpace(size, address)) {
        Serial.println(F("Not enough space."));
        return false;
    }

    // Write info.
    for (int i = size - 1; i >= 0; i--) {
        byte temp;
        if (!popByte(temp, stack, false)) {
            Serial.println(F("Invalid variable."));
            return false;
        }
        memory[address + i] = temp;
    }

    // Write new mem entry.
    writeMemEntry({name, type, address, process});
    return true;
}

bool getVar(char name, int process, stack &stack) {
    // Check amount of variables.
    if (noOfVars == 0) {
        Serial.println(F("No variables."));
        return false;
    }

    // Check for existence.
    int varId = varExists(name, process);
    if (varId == -1) {
        Serial.println(F("Var does not exist."));
        return false;
    }

    memEntry entry = variables[varId];

    int type = entry.type;
    int address = entry.address;

    if (type != STRING) {
        for (int i = 0; i < type; i++) {
            if (!pushByte(memory[address + i], stack)) {
                return false;
            }
        }
    } else {
        int size = 0;
        while (memory[address + size] != 0) {
            if (!pushByte(memory[address + size++], stack)) {
                return false;
            }
        }
        if (!pushByte(memory[address + size], stack)) {
            return false;
        }
        if (!pushByte(++size, stack)) {
            return false;
        }
    }
    if (!pushByte(type, stack)) {
        return false;
    }
    return true;
}

void wipeVars(int process) {
    int size = noOfVars;
    for (int i = size - 1; i >= 0; i--) {
        if (variables[i].process == process) {
            deleteMemEntry(i);
        }
    }
}

void memDump(stack stack) {
    Serial.println(stack.sp);

    for (int i = 0; i < stack.sp; i++) {
        Serial.print(i);
        Serial.print(": ");
        Serial.println(stack.stack[i]);
    }

    Serial.println();
}

bool pushInt(int i, stack &stack) {
    if (stack.sp > STACKSIZE - 4) {
        return false;
    }
    pushByte(highByte(i), stack);
    pushByte(lowByte(i), stack);
    pushByte(INT, stack);
    return true;
}

bool pushFloat(float f, stack &stack) {
    if (stack.sp > STACKSIZE - 6) {
        return false;
    }
    byte b[4];
    auto *pf = (float *)b;
    *pf = f;
    for (int i = 3; i >= 0; i--) {
        pushByte(b[i], stack);
    }
    pushByte(FLOAT, stack);
    return true;
}

bool popInt(int &i, stack &stack, bool peek) {
    if (stack.sp < INT) {
        return false;
    }
    byte hb;
    byte lb;

    popByte(lb, stack, false);
    if (peek) {
        popByte(hb, stack, true);
        pushByte(lb, stack);
    } else {
        popByte(hb, stack, false);
    }
    Serial.println(lb);
    Serial.println(hb);
    i = word(hb, lb);
    return true;
}

bool popFloat(float &f, stack &stack, bool peek) {
    if (stack.sp < INT) {
        return false;
    }
    byte b[4];
    auto *pf = (float *)b;

    for (byte &i : b) {
        popByte(i, stack, false);
    }

    if (peek) {
        for (int i = 3; i >= 0; i--) {
            pushByte(b[i], stack);
        }
    }

    f = *pf;
    return true;
}

bool popVal(float &f, stack &stack, bool peek) {
    byte type;
    popByte(type, stack, false);

    switch (type) {
        case CHAR:
            byte tChar;
            if (!popByte(tChar, stack, peek)) {
                return false;
            }
            f = tChar;
            break;
        case INT:
            int tInt;
            if (!popInt(tInt, stack, peek)) {
                return false;
            }
            f = tInt;
            break;
        case FLOAT:
            if (!popFloat(f, stack, peek)) {
                return false;
            }
            break;
        default:
            Serial.println(F("Type not recognized."));
            return false;
    }

    if (peek) {
        pushByte(type, stack);
    }
    return true;
}

bool popString(int &cp, stack &stack) {
    byte length;
    int &sp = stack.sp;
    if (!popByte(length, stack, false)) {
        return false;
    }
    if (length > sp) {
        return false;
    }

    for (; length > 0; length--) {
        byte temp;
        popByte(temp, stack, false);
    }

    cp = sp;
    return true;
}
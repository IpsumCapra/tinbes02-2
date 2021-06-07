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

bool popByte(byte &b, stack &stack) {
    int &sp = stack.sp;
    if (sp > 0) {
        b = stack.stack[--sp];
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
        Serial.println("Max amount reached.");
        return false;
    }

    // Get variable type from stack.
    byte type;
    if (!popByte(type, stack)) {
        Serial.println("Stack not valid");
        return false;
    }

    // Determine variable size.
    byte size;
    if (type != STRING) {
        size = type;
    } else {
        if (!popByte(size, stack)) {
            Serial.println("No string size found.");
            return false;
        }
    }

    // Try to allocate space.
    int address;
    if(!findSpace(size, address)) {
        Serial.println("Not enough space.");
        return false;
    }

    // Write info.
    for (int i = size - 1; i >= 0; i--) {
        byte temp;
        if (!popByte(temp, stack)) {
            Serial.println("Invalid variable.");
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
        Serial.println("No variables.");
        return false;
    }

    // Check for existence.
    int varId = varExists(name, process);
    if (varId == -1) {
        Serial.println("Var does not exist.");
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
    for (int i = 0; i < noOfVars; i++) {
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
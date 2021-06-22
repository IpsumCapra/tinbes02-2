#include <processes.h>
#include <filesystem.h>
#include <executor.h>

processEntry processes[MAXPROCESSES];
int procCount = 0;
int procID = 0;

int retrieveProcess(int id) {
    for (int i = 0; i < procCount; i++) {
        if (processes[i].id == id) {
            return i;
        }
    }
    return -1;
}

bool runProcess(char name[]) {
    if (procCount == MAXPROCESSES) {
        Serial.println(F("No space."));
        return false;
    }
    int entry = lookupEntry(name);
    if (entry == -1) {
        Serial.println(F("File not found."));
    }
    FATEntry fEntry;
    if (!readFATEntry(entry, fEntry)) {
        return false;
    }

    processEntry pEntry;
    strcpy(pEntry.name, name);
    pEntry.state = RUNNING;
    pEntry.pc = fEntry.start;
    pEntry.id = procID++;

    processes[procCount++] = pEntry;
    Serial.print(F("Successfully started "));
    Serial.println(pEntry.id);
    return true;
}

String printState(byte state) {
    switch (state) {
        case RUNNING:
            return "RUNNING";
        case PAUSED:
            return "PAUSED";
        case TERMINATED:
            return "TERMINATED";
        default:
            return "UNKNOWN";
    }
}

bool setState(int i, byte state) {
    int id = retrieveProcess(i);
    if (id == -1) {
        Serial.println(F("Process non-existent."));
        return false;
    }

    byte &procState = processes[id].state;
    if (procState == state) {
        Serial.print("Already " + printState(state));
        return false;
    } else {
        procState = state;
        Serial.println("State now " + printState(state));
        return true;
    }
}

bool killProcess(int i) {
    if (!setState(i, TERMINATED)) {
        return false;
    }
    wipeVars(i);
    return true;
}

void listProcesses() {
    for (int i = 0; i < procCount; i++) {
        Serial.print(F("ID: "));
        Serial.println(processes[i].id);
        Serial.print(F("State: "));
        Serial.println(printState(processes[i].state));
        Serial.print(F("Name: "));
        Serial.println(processes[i].name);
    }
}

void runProcesses() {
    for (int i = 0; i < procCount; i++) {
        processEntry &pEntry = processes[i];
        if (pEntry.state == RUNNING) {
            execute(pEntry);
        }
    }
}

int getCurrentProcID() {
    return procCount - 1;
}

byte getProcessState(int id) {
    for (int i = 0; i < procCount; i++) {
        if (processes[i].id == id) {
            return processes[i].state;
        }
    }
    return -1;
}
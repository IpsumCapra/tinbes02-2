#include <cli_commands.h>
#include <filesystem.h>
#include <processes.h>
#include <Arduino.h>

void store(Parameters pars) {
    int size = strtol(pars[1], NULL, 10);

    if(storeFile(pars[0], size)) {
        Serial.println(F("File stored."));
    } else {
        Serial.println(F("Could not store file."));
    }
}

void retrieve(Parameters pars) {
    if(!retrieveFile(pars[0])) {
        Serial.println(F("Could not retrieve file."));
    }
}

void erase(Parameters pars) {
    if(deleteFile(pars[0])) {
        Serial.println(F("File deleted."));
    } else {
        Serial.println(F("Could not delete file."));
    }
}

void files(Parameters pars) {
    listFiles();
}

void freespace(Parameters pars) {
    Serial.print(F("Free space: "));
    Serial.print(freeSpace());
    Serial.println(F(" bytes"));
}

void run(Parameters pars) {
    if(!runProcess(pars[0])) {
        Serial.println(F("Failed to run."));
    }
}

void list(Parameters pars) {
    listProcesses();
}

void suspend(Parameters pars) {
    int id = strtol(pars[0], NULL, 10);
    setState(id, PAUSED);
}

void resume(Parameters pars) {
    int id = strtol(pars[0], NULL, 10);
    setState(id, RUNNING);
}

void kill(Parameters pars) {
    int id = strtol(pars[0], NULL, 10);
    killProcess(id);
}

void reformat(Parameters pars) {
    EEPROM[0] = 0;
    for (int i = 1; i < EEPROM.length(); i++) {
        EEPROM[i] = 255;
    }
    Serial.println(F("Reformatted."));
}
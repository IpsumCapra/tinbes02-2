#include <cli_commands.h>
#include <filesystem.h>
#include <Arduino.h>

void store(Parameters pars) {
    int size = 0;
    for (int i = 0; i < sizeof(pars[1]); i++) {
        if (isDigit(pars[1][i])) {
            int ic = pars[1][i] - '0';
            size += ic * pow(10, i);
        }
    }

    if(storeFile(pars[0], size)) {
        Serial.println("File stored successfully.");
    } else {
        Serial.println("Could not store file.");
    }
}

void retrieve(Parameters pars) {
    if(!retrieveFile(pars[0])) {
        Serial.println("Could not retrieve file.");
    }
}

void erase(Parameters pars) {
    Serial.println("erase");
    Serial.println(pars[0]);
}

void files(Parameters pars) {
    listFiles();
}

void freespace(Parameters pars) {
    Serial.print("Free space: ");
    Serial.print(freeSpace());
    Serial.println(" bytes");
}

void run(Parameters pars) {
    Serial.println("run");
    Serial.println(pars[0]);
}

void list(Parameters pars) {
    Serial.println("list");
}

void suspend(Parameters pars) {
    Serial.println("suspend");
    Serial.println(pars[0]);
}

void resume(Parameters pars) {
    Serial.println("resume");
    Serial.println(pars[0]);
}

void kill(Parameters pars) {
    Serial.println("kill");
    Serial.println(pars[0]);
}

void reformat(Parameters pars) {
    EEPROM[0] = 0;
    for (int i = 1; i < EEPROM.length(); i++) {
        EEPROM[i] = 255;
    }
    Serial.println("Reformat complete.");
}
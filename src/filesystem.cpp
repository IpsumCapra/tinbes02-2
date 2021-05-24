#include <filesystem.h>
#include <Arduino.h>
#include <EEPROM.h>

EERef noOfFiles = EEPROM[0];


void setupFS() {
    Serial.print("EEPROM size: ");
    Serial.println(EEPROM.length());

    // Check if there is a FS structure available, create one if there isn't.
    if (noOfFiles > 10) {
        Serial.println("Creating filesystem.");
        noOfFiles = 0;
    } else if (noOfFiles > 0) {
        Serial.print(noOfFiles);
        Serial.println(" files found.");
    }
    Serial.println("Filesystem initialized.");
}

bool writeFATEntry(FATEntry entry) {
    return true;
}

bool readFATEntry(int start, FATEntry& entry) {
    return false;
}
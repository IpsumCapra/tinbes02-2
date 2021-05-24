#include <Arduino.h>
#include <EEPROM.h>
#include <cli.h>
#include <filesystem.h>
#include <constants.h>

void setup() {
    // Setup serial.
    Serial.begin(BAUD_RATE);
    Serial.println("Serial ready.");

    // Setup filesystem.
    setupFS();

    Serial.println("\nArduinOS RDY");
}

void loop() {
    processInput();
}
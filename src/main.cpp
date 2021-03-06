#include <Arduino.h>
#include <cli.h>
#include <filesystem.h>
#include <constants.h>
#include <processes.h>

void setup() {
    // Setup serial.
    Serial.begin(BAUD_RATE);
    Serial.setTimeout(-1);
    Serial.println(F("Serial ready."));

    // Setup filesystem.
    setupFS();

    Serial.println(F("\nArduinOS RDY"));
}

void loop() {
    processInput();
    runProcesses();
}
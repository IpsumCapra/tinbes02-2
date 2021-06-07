#include <Arduino.h>
#include <cli.h>
#include <filesystem.h>
#include <constants.h>
#include <memory.h>

void setup() {
    // Setup serial.
    Serial.begin(BAUD_RATE);
    Serial.setTimeout(-1);
    Serial.println("Serial ready.");

    // Setup filesystem.
    setupFS();

    Serial.println("\nArduinOS RDY");

    stack stack;

    pushByte('H', stack);
    pushByte('e', stack);
    pushByte('l', stack);
    pushByte('l', stack);
    pushByte('o', stack);
    pushByte(0, stack);
    pushByte(6, stack);
    pushByte(STRING, stack);

    memDump(stack);

    setVar('s', 2, stack);

    memDump(stack);

    getVar('s', 2, stack);

    memDump(stack);
}

void loop() {
    processInput();
}
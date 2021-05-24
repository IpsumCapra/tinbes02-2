#include <Arduino.h>
#include <cli.h>
#include <constants.h>

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("ArduinOS RDY");
}

void loop() {
    processInput();
}
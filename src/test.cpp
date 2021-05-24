#include <test.h>

void store(Parameters pars) {
    Serial.println("store");
    Serial.println(pars[0]);
    Serial.println(pars[1]);
    Serial.println(pars[2]);
}

void retrieve(Parameters pars) {
    Serial.println("retrieve");
    Serial.println(pars[0]);
}

void erase(Parameters pars) {
    Serial.println("erase");
    Serial.println(pars[0]);
}

void files(Parameters pars) {
    Serial.println("files");
}

void freespace(Parameters pars) {
    Serial.println("freespace");
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
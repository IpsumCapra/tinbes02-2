#include <Arduino.h>
#include <constants.h>
#include <cli.h>

char commandBuffer[CBUFSIZE] = {};
int commandBufferPointer = 0;

int tokenCount(char string[]) {
    int count = 0;
    char str[CBUFSIZE];
    strcpy(str, string);

    char *token = strtok(str, " ");

    while (token != NULL) {
        token = strtok(NULL, " ");
        count++;
    }

    return count;
}

// Handle complete command.
bool handleCommand() {
    Serial.println(commandBuffer);

    int size = tokenCount(commandBuffer);
    if (size == 0) {
        Serial.println();
        return true;
    }

    char temp[CBUFSIZE];
    strcpy(temp, commandBuffer);
    char *name = strtok(temp, " ");

    for (int i = 0; i < sizeof(commands) - 1; i++) {
        if (strcmp(commands[i].name, name) == 0) {
            if (size == 1) {
                commands[i].callback(NULL);
            } else {
                char pars[size][BUFSIZE];
                char *token = strtok(commandBuffer, " ");
                int ptr = 0;
                while (token != NULL) {
                    token = strtok(NULL, " ");
                    strcpy(pars[ptr], token);
                    ptr++;
                }
                commands[i].callback(pars);
            }
            return true;
        }
    }
    return false;
}

// Read incoming data, send formulated commands to handler.
void processInput() {
    // Only process if there's something to read.
    if (Serial.available()) {
        char temp = Serial.read();
        if (commandBufferPointer < CBUFSIZE - 1) {
            commandBuffer[commandBufferPointer] = temp;
            commandBufferPointer++;
        }

        if (temp == '\n') {
            // Try to append a space to make sure first token is always found.
            if (commandBufferPointer < CBUFSIZE - 1) {
                commandBuffer[commandBufferPointer - 1] = ' ';
            }
            if (!handleCommand()) {
                Serial.println("Command not recognized");
            }

            // Clear command buffer.
            commandBufferPointer = 0;
            memset(commandBuffer, 0, sizeof(commandBuffer));
        }
    }
}

// Command constructor
Command::Command(const char name[BUFSIZE], Callback callback, int parameters) {
    for (int i = 0; i < BUFSIZE; i++) {
        this->name[i] = name[i];
    }
    this->callback = callback;
    this->parameters = parameters;
}

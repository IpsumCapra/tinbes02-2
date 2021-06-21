#include <executor.h>
#include <EEPROM.h>
#include <instruction_set.h>
#include <Arduino.h>

// Push char, int or float to stack.
void pushVal(processEntry proc) {
    stack &stack = proc.stack;
    int &pc = proc.pc;

    byte type = EEPROM[pc++];

    for (int i = 0; i < type; i++) {
        pushByte(EEPROM[pc++],stack);
    }
    pushByte(type, stack);
}

// Push string to stack.
void pushString(processEntry proc) {
    stack &stack = proc.stack;
    int &pc = proc.pc;

    // Type not needed. Advance PC.
    pc++;

    int size = 1;

    while (EEPROM[pc++] != 0x00) {
        pushByte(EEPROM[pc], stack);
        size++;
    }

    pushByte(0x00, stack);
    pushByte(size, stack);
    pushByte(STRING, stack);
}

// Print variable with or without newline.
void print(processEntry proc, bool newLine) {
    stack &stack = proc.stack;
    int &pc = proc.pc;

    pc++;
    byte type;
    popByte(type, stack, true);

    if (type != STRING) {
        float val;
        popVal(val, stack, false);
        if (type == CHAR) {
            Serial.print((char)val);
        } else {
            Serial.print(val);
        }
    } else {
        popByte(type, stack, false);
        int cp;
        popString(cp, stack);
        while (stack.stack[cp++] != 0x00) {
            Serial.print((char)stack.stack[cp]);
        }
    }
    if (newLine) {
        Serial.print("\n");
    }
}

// Function to handle all unary operations.
void unaryOperation(processEntry proc) {
    stack &stack = proc.stack;
    int &pc = proc.pc;

    // Instruction, and variable type.
    byte type = EEPROM[pc++];
    byte vType;
    popByte(vType, stack, true);

    // Retrieve value.
    float val;
    popVal(val, stack, false);

    // Return type.
    byte rType = vType;

    switch (type) {
        case INCREMENT:
            val++;
            break;
        case DECREMENT:
            val--;
            break;
        case UNARYMINUS:
            val = -val;
            break;
        case ABS:
            val = abs(val);
            break;
        case SQ:
            val = val*val;
            break;
        case SQRT:
            val = sqrt(val);
            break;
        case ANALOGREAD:
            val = analogRead(val);
            rType = INT;
            break;
        case DIGITALREAD:
            val = digitalRead(val);
            rType = CHAR;
            break;
        case LOGICALNOT:
            val = val == 0 ? 1 : 0;
            rType = CHAR;
            break;
        case BITWISENOT:
            val = ~(int)val;
            break;
        case TOCHAR:
            val = (char)val;
            rType = CHAR;
            break;
        case TOINT:
            val = (int)val;
            rType = INT;
            break;
        case TOFLOAT:
            rType = FLOAT;
            break;
        case ROUND:
            val = round(val);
            rType = INT;
            break;
        case FLOOR:
            val = floor(val);
            rType = INT;
            break;
        case CEIL:
            val = ceil(val);
            rType = INT;
            break;
    }

    switch (rType) {
        case CHAR:
            pushByte(val, stack);
            pushByte(rType, stack);
        case INT:
            pushInt(val, stack);
        case FLOAT:
            pushFloat(val, stack);
    }
}


// Function to handle all binary operations.
void binaryOperation(processEntry proc) {
    stack &stack = proc.stack;
    int &pc = proc.pc;

    // Instruction, and variable type.
    byte type = EEPROM[pc++];
    byte vTypeY;
    popByte(vTypeY, stack, true);

    // Retrieve value.
    float y;
    popVal(y, stack, false);

    // Retrieve type of x.
    byte vTypeX;
    popByte(vTypeX, stack, true);

    // Retrieve value.
    float x;
    popVal(x, stack, false);

    // Return type.
    byte rType = vTypeX > vTypeY ? vTypeX : vTypeY;

    // Return value.
    float val;

    switch (type) {
        case PLUS:
            val = x + y;
            break;
        case MINUS:
            val = x - y;
            break;
        case TIMES:
            val = x * y;
            break;
        case DIVIDEDBY:
            val = x / y;
            break;
        case MODULUS:
            val = (int)x % (int)y;
            break;
        case EQUALS:
            val = x == y;
            rType = CHAR;
            break;
        case NOTEQUALS:
            val = x != y;
            rType = CHAR;
            break;
        case LESSTHAN:
            val = x < y;
            rType = CHAR;
            break;
        case LESSTHANOREQUALS:
            val = x <= y;
            rType = CHAR;
            break;
        case GREATERTHAN:
            val = x > y;
            rType = CHAR;
            break;
        case GREATERTHANOREQUALS:
            val = x >= y;
            rType = CHAR;
            break;
        case MIN:
            val = x < y ? x : y;
            rType = CHAR;
            break;
        case MAX:
            val = x > y ? x : y;
            rType = CHAR;
            break;
        case POW:
            val = pow(x, y);
            break;
        case LOGICALAND:
            val = x && y;
            rType = CHAR;
            break;
        case LOGICALOR:
            val = x || y;
            rType = CHAR;
            break;
        case LOGICALXOR:
            val = !(x)!= !(y);
            rType = CHAR;
            break;
        case BITWISEAND:
            val = (bool)x & (bool)y;
            break;
        case BITWISEOR:
            val = (bool)x | (bool)y;
            break;
        case BITWISEXOR:
            val = (bool)x ^ (bool)y;
            break;
    }

    switch (rType) {
        case CHAR:
            pushByte(val, stack);
            pushByte(rType, stack);
        case INT:
            pushInt(val, stack);
        case FLOAT:
            pushFloat(val, stack);
    }
}

// Find next instruction to execute.
void execute(processEntry proc) {
    stack stack = proc.stack;
    int &pc = proc.pc;

    byte instruction = EEPROM[pc];
    switch (instruction) {
        case CHAR:
        case INT:
        case FLOAT:
            pushVal(proc);
            break;
        case STRING:
            pushString(proc);
            break;
        case PRINT:
            print(proc, false);
            break;
        case PRINTLN:
            print(proc, true);
            break;
        case GET:
            pc++;
            getVar(EEPROM[pc++], proc.id, proc.stack);
            break;
        case SET:
            pc++;
            setVar(EEPROM[pc++], proc.id, proc.stack);
            break;
        case INCREMENT:
        case DECREMENT:
        case UNARYMINUS:
        case ABS:
        case SQ:
        case SQRT:
        case ANALOGREAD:
        case DIGITALREAD:
        case LOGICALNOT:
        case BITWISENOT:
        case TOCHAR:
        case TOINT:
        case TOFLOAT:
        case ROUND:
        case FLOOR:
        case CEIL:
            unaryOperation(proc);
            break;
        case PLUS:
        case MINUS:
        case TIMES:
        case DIVIDEDBY:
        case MODULUS:
        case EQUALS:
        case NOTEQUALS:
        case LESSTHAN:
        case LESSTHANOREQUALS:
        case GREATERTHAN:
        case GREATERTHANOREQUALS:
        case MIN:
        case MAX:
        case POW:
        case LOGICALAND:
        case LOGICALOR:
        case LOGICALXOR:
        case BITWISEAND:
        case BITWISEOR:
        case BITWISEXOR:
            binaryOperation(proc);
            break;
        case STOP:
            wipeVars(proc.id);
            setState(proc.id, TERMINATED);
            break;
        case IF: {
            pc++;
            byte ifByte;
            popByte(ifByte, stack, true);
            if (ifByte == 0) {
                pc += EEPROM[pc];
            }
            break;
        }
        case ELSE: {
            pc++;
            byte elseByte;
            popByte(elseByte, stack, true);
            if (elseByte != 0) {
                pc += EEPROM[pc];
            }
            break;
        }
        case LOOP:
            proc.lp = pc++;
            break;
        case ENDLOOP:
            pc = proc.lp;
            break;
        case WHILE: {
            pc++;
            byte wArg1 = EEPROM[pc++];
            byte wArg2 = EEPROM[pc++];
            float whileVal;
            popVal(whileVal, stack, false);
            if (whileVal == 0) {
                pc += wArg2 + 1;
            } else {
                pushByte(wArg1 + wArg2 + 4, stack);
            }
            break;
        }
        case ENDWHILE: {
            pc++;
            float eWhileArg;
            popVal(eWhileArg, stack, false);
            pc -= eWhileArg;
            break;
        }
        case DELAY: {
            pc++;
            float delayVal;
            popVal(delayVal, stack, false);
            delay(delayVal);
            break;
        }
        case MILLIS:
            pc++;
            pushInt(millis(), stack);
            break;
        case DELAYUNTIL: {
            pc++;
            float dUntilVal;
            popVal(dUntilVal, stack, false);
            if (dUntilVal > millis()) {
                pc--;
                pushFloat(dUntilVal, stack);
            }
        }

    }
}
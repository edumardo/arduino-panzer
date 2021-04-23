#include "Utilities.h"

/**
 *
 */
void setPWMPrescaler(uint8_t pin, uint16_t prescale) {

    byte mode;

    if (pin == 9 || pin == 10) {
        switch (prescale) {
            case    1: mode = 0b001; break;
            case    8: mode = 0b010; break;
            case   32: mode = 0b011; break;
            case   64: mode = 0b100; break;
            case  128: mode = 0b101; break;
            case  256: mode = 0b110; break;
            case 1024: mode = 0b111; break;
            default: return;
        }
    } else {
        switch(prescale) {
            case    1: mode = 0b001; break;
            case    8: mode = 0b010; break;
            case   64: mode = 0b011; break;
            case  256: mode = 0b100; break;
            case 1024: mode = 0b101; break;
            default: return;
        }
    }

    if (pin == 4 || pin == 13 ) {
        TCCR0B = TCCR0B & 0b11111000 | mode;
    } else if (pin == 11 || pin == 12 ) {
        TCCR1B = TCCR1B & 0b11111000 | mode;
    } else if (pin == 9 || pin == 10 ) {
        TCCR2B = TCCR2B & 0b11111000 | mode;
    } else if (pin == 2 || pin ==  3 || pin == 5) {
        TCCR3B  = TCCR3B & 0b11111000 | mode;
    } else if (pin == 6 || pin ==  7 || pin == 8) {
        TCCR4B  = TCCR4B & 0b11111000 | mode;
    } else if (pin == 44 || pin == 45 || pin == 46) {
        TCCR5B = TCCR5B & 0b11111000 | mode;
    }
}

/**
 *
 */
void printSpaces(int n) {

    for (int i = 0; i < n; i++) {
        Serial.print(" ");
    }
}

/**
 *
 */
void printByte(byte b) {

    if (b < 10) {
        printSpaces(2);
    } else if (b < 100) {
        printSpaces(1);
    }
    Serial.print(b);
}

/**
 *
 */
void printBool(bool value) {

    Serial.print(value ? "true" : "false");
}

/**
 *
 */
void printAnalogController(String name, byte x, byte y) {

        Serial.print("[");
        Serial.print(name);
        Serial.print(" ");
        printByte(x);
        Serial.print(" | ");
        printByte(y);
        Serial.print("]");
}

/**
 *
 */
void printUDLRController(byte up, byte down, byte left, byte right) {

        Serial.print("[");
        Serial.print("UDLR");
        Serial.print(" ");
        printByte(up);
        Serial.print(" | ");
        printByte(down);
        Serial.print(" | ");
        printByte(left);
        Serial.print(" | ");
        printByte(right);
        Serial.print("]");
}

/**
 *
 */
void printLabelBoolValue(String label, bool value) {

        Serial.print("[");
        Serial.print(label);
        Serial.print(" ");
        printBool(value);
        Serial.print("]");
}

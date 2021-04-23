#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

void setPWMPrescaler(uint8_t pin, uint16_t prescale);
void printSpaces(int n);
void printByte(byte b);
void printBool(bool value);
void printAnalogController(String name, byte x, byte y);
void printUDLRController(byte up, byte down, byte left, byte right);
void printLabelBoolValue(String label, bool value);

#endif

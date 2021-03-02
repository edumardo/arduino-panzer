#ifndef PINOUT_H
#define PINOUT_H

/* PS2 pins */
#define PS2_PIN_DAT A2
#define PS2_PIN_CMD A1
#define PS2_PIN_SEL A0
#define PS2_PIN_CLK A3
const bool PS2_PRESSURES = true;
const bool PS2_RUMBLE    = true;

/* TB6612FNG drive pins */
const int TB_DRIVE_PWMA = 3;
const int TB_DRIVE_AIN2 = 2;
const int TB_DRIVE_AIN1 = 4;
const int TB_DRIVE_STBY = 8;
const int TB_DRIVE_BIN1 = 12;
const int TB_DRIVE_BIN2 = 7;
const int TB_DRIVE_PWMB = 9;

#endif

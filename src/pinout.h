#ifndef PINOUT_H
#define PINOUT_H

/* PS2 pins */
const int PS2_PIN_DAT    = 12;
const int PS2_PIN_CMD    = 11;
const int PS2_PIN_SEL    = 10;
const int PS2_PIN_CLK    = 13;
const bool PS2_PRESSURES = true;
const bool PS2_RUMBLE    = true;

/* TB6612FNG pins */
const int TB_PWMA = 3;
const int TB_AIN2 = 2;
const int TB_AIN1 = 4;
const int TB_STBY = 5;
const int TB_BIN1 = 6;
const int TB_BIN2 = 7;
const int TB_PWMB = 9;

/* Led hull MG pin */
#define LED_HULLMG_PIN  A2

#endif

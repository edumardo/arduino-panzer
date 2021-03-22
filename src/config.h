#ifndef CONFIG_H
#define CONFIG_H

/* Center value of the transmitter stick */
const int CENTER_STICK_VALUE = 128;

/* Minimal value of the transmitter stick */
const int MIN_STICK_VALUE = 0; 

/* Maximal value of the transmitter stick */
const int MAX_STICK_VALUE = 255;

/* PS2 pins */
#define PS2_PIN_DAT A2
#define PS2_PIN_CMD A1
#define PS2_PIN_SEL A0
#define PS2_PIN_CLK A3
const bool PS2_PRESSURES = true;
const bool PS2_RUMBLE    = true;

/* TB6612FNG Drive pins */
const int TB_DRIVE_PWMA =  4;
const int TB_DRIVE_AIN2 = 23;
const int TB_DRIVE_AIN1 = 25;
const int TB_DRIVE_STBY = 27;
const int TB_DRIVE_BIN1 = 29;
const int TB_DRIVE_BIN2 = 31;
const int TB_DRIVE_PWMB = 13;

/* TB6612FNG Turret rotation and gun elevation pins */
const int TB_TURRETROTATION_PWMA = 11;
const int TB_TURRETROTATION_AIN2 = 22;
const int TB_TURRETROTATION_AIN1 = 24;
const int TB_TURRETGUN_STBY      = 26;
const int TB_GUNELEVATION_BIN1   = 28;
const int TB_GUNELEVATION_BIN2   = 30;
const int TB_GUNELEVATION_PWMB   = 12;

/* Airsoft gun */
const int AIRSOFT_FIRED_INTERRUPT_PIN = 18;

#endif

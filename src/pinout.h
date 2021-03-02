#ifndef PINOUT_H
#define PINOUT_H

/* PS2 pins */
#define PS2_PIN_DAT A2
#define PS2_PIN_CMD A1
#define PS2_PIN_SEL A0
#define PS2_PIN_CLK A3
const bool PS2_PRESSURES = true;
const bool PS2_RUMBLE    = true;

/* TB6612FNG Drive pins */
const int TB_DRIVE_PWMA = 3;
const int TB_DRIVE_AIN2 = 2;
const int TB_DRIVE_AIN1 = 4;
const int TB_DRIVE_STBY = 8;
const int TB_DRIVE_BIN1 = 12;
const int TB_DRIVE_BIN2 = 7;
const int TB_DRIVE_PWMB = 9;

/* TB6612FNG Turret rotation and gun elevation pins */
const int TB_TURRETROTATION_PWMA = 5;
#define TB_TURRETROTATION_AIN2    A5
#define TB_TURRETROTATION_AIN1    A4
const int TB_TURRETGUN_STBY =      6;
const int TB_GUNELEVATION_BIN1 =  10;
const int TB_GUNELEVATION_BIN2 =  13;
const int TB_GUNELEVATION_PWMB =  11;

#endif

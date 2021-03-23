#ifndef CONFIG_H
#define CONFIG_H

/* Transmitter stick */
const int MIN_STICK_VALUE    =   0; 
const int CENTER_STICK_VALUE = 128;
const int MAX_STICK_VALUE    = 255;

/* PS2 pins */
const uint8_t PS2_PIN_DAT =   A2;
const uint8_t PS2_PIN_CMD =   A1;
const uint8_t PS2_PIN_SEL =   A0;
const uint8_t PS2_PIN_CLK =   A3;
const bool PS2_PRESSURES  = true;
const bool PS2_RUMBLE     = true;

/* TB6612FNG Drive pins */
const uint8_t TB_DRIVE_APWM_PRESCALER =  1;
const uint8_t TB_DRIVE_APWM           =  9;
const uint8_t TB_DRIVE_AIN2           = 23;
const uint8_t TB_DRIVE_AIN1           = 25;
const uint8_t TB_DRIVE_STBY           = 27;
const uint8_t TB_DRIVE_BIN1           = 29;
const uint8_t TB_DRIVE_BIN2           = 31;
const uint8_t TB_DRIVE_BPWM           = 10;
const uint8_t TB_DRIVE_BPWM_PRESCALER =  1;

/* TB6612FNG Turret rotation and gun elevation pins */
const uint8_t TB_TURRETROTATION_PWM_PRESCALER =  1;
const uint8_t TB_TURRETROTATION_PWM           = 11;
const uint8_t TB_TURRETROTATION_IN2           = 22;
const uint8_t TB_TURRETROTATION_IN1           = 24;
const uint8_t TB_TURRETGUN_STBY               = 26;
const uint8_t TB_GUNELEVATION_IN1             = 28;
const uint8_t TB_GUNELEVATION_IN2             = 30;
const uint8_t TB_GUNELEVATION_PWM             = 12;
const uint8_t TB_GUNELEVATION_PWM_PRESCALER   =  1;

/* TB6612FNG Smoke generator and fan pins */
const uint8_t TB_SMOKE_GENERATOR_PWM_PRESCALER =  1;
const uint8_t TB_SMOKE_GENERATOR_PWM           =  2;
const uint8_t TB_SMOKE_GENERATOR_AIN2          = 32;
const uint8_t TB_SMOKE_GENERATOR_AIN1          = 33;
const uint8_t TB_SMOKE_GENERATORFAN_STBY       = 34;
const uint8_t TB_SMOKE_FAN_IN1                 = 35;
const uint8_t TB_SMOKE_FAN_IN2                 = 36;
const uint8_t TB_SMOKE_FAN_PWM                 =  3;
const uint8_t TB_SMOKE_FAN_PWM_PRESCALER       =  1;

/* Dasmikro TBS Mini sound unit */
const uint8_t TBSMINI_PROP1 = 49;
const uint8_t TBSMINI_PROP2 = 51;
const uint8_t TBSMINI_PROP3 = 53;

/* Airsoft gun */
const uint8_t AIRSOFT_FIRED_INTERRUPT_PIN = 18;

#endif

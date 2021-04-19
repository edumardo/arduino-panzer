#ifndef CONFIG_H
#define CONFIG_H

#include <PS2X_lib.h>
#include "SmokeController.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------
// PS2 Radio
//--------------------------------------------------------------------------------------------------------------------------------------------------

// Miliseconds between reads
const int PS2_MS_BETWEEN_READS = 50;

// Stick properties
const int PS2_MIN_STICK_VALUE    =   0;
const int PS2_CENTER_STICK_VALUE = 128;
const int PS2_MAX_STICK_VALUE    = 255;

// PS2 pins
const uint8_t PS2_DAT =   A2;
const uint8_t PS2_CMD =   A1;
const uint8_t PS2_SEL =   A0;
const uint8_t PS2_CLK =   A3;
const bool PS2_PRESSURES  = true;
const bool PS2_RUMBLE     = true;

// PS2 button abstraction
#define CONTROLLER_TURRET_ROTATION       PSS_RX
#define CONTROLLER_GUN_ELEVATION         PSS_RY
#define CONTROLLER_STEERING_THROTTLE     PSS_LX
#define CONTROLLER_STEERING_TURN         PSS_LY
#define CONTROLLER_DRIVE_THROTTLE        PSAB_PAD_UP
#define CONTROLLER_BUTTON_DRIVE_THROTTLE PSB_PAD_UP
#define CONTROLLER_DRIVE_REVERSE         PSAB_PAD_DOWN
#define CONTROLLER_BUTTON_DRIVE_REVERSE  PSB_PAD_DOWN
#define CONTROLLER_DRIVE_LEFT            PSAB_PAD_LEFT
#define CONTROLLER_BUTTON_DRIVE_LEFT     PSB_PAD_LEFT
#define CONTROLLER_DRIVE_RIGHT           PSAB_PAD_RIGHT
#define CONTROLLER_BUTTON_DRIVE_RIGHT    PSB_PAD_RIGHT
#define CONTROLLER_FIRE_GUN              PSB_PINK
#define CONTROLLER_STARTSTOP             PSB_START
#define CONTROLLER_VOLUMEUP              PSB_L2
#define CONTROLLER_VOLUMEDOWN            PSB_R2

// Miliseconds to debounce buttons
const int CONTROLLER_MS_DEBOUNCE = 500;
//--------------------------------------------------------------------------------------------------------------------------------------------------
// Drive direction
//--------------------------------------------------------------------------------------------------------------------------------------------------

// TB6612FNG Drive pins
const uint8_t TB_DRIVE_APWM_PRESCALER =  1;
const uint8_t TB_DRIVE_APWM           =  9;
const uint8_t TB_DRIVE_AIN2           = 23;
const uint8_t TB_DRIVE_AIN1           = 25;
const uint8_t TB_DRIVE_STBY           = 27;
const uint8_t TB_DRIVE_BIN1           = 29;
const uint8_t TB_DRIVE_BIN2           = 31;
const uint8_t TB_DRIVE_BPWM           = 10;
const uint8_t TB_DRIVE_BPWM_PRESCALER =  1;

// Max voltage percent sent to drive
const uint8_t DRIVE_MAX_VOLTAGE_PERCENT_ANALOG = 100;
const uint8_t DRIVE_MAX_VOLTAGE_PERCENT_PAD    =  80;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Turret rotation and gun elevation
//--------------------------------------------------------------------------------------------------------------------------------------------------

// TB6612FNG Turret rotation and gun elevation pins
const uint8_t TB_TURRETROTATION_PWM_PRESCALER =  1;
const uint8_t TB_TURRETROTATION_PWM           = 11;
const uint8_t TB_TURRETROTATION_IN2           = 22;
const uint8_t TB_TURRETROTATION_IN1           = 24;
const uint8_t TB_TURRETGUN_STBY               = 26;
const uint8_t TB_GUNELEVATION_IN1             = 28;
const uint8_t TB_GUNELEVATION_IN2             = 30;
const uint8_t TB_GUNELEVATION_PWM             = 12;
const uint8_t TB_GUNELEVATION_PWM_PRESCALER   =  1;

// Max voltage percent sent to rotation and elevation motors
const uint8_t TURRETROTARION_MAX_VOLTAGE_PERCENT = 100;
const uint8_t GUNELEVATION_MAX_VOLTAGE_PERCENT   = 100;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Smoke generator and fan
//--------------------------------------------------------------------------------------------------------------------------------------------------

// TB6612FNG Smoke generator and fan pins
const uint8_t TB_SMOKE_GENERATOR_PWM_PRESCALER =  1;
const uint8_t TB_SMOKE_GENERATOR_PWM           =  2;
const uint8_t TB_SMOKE_GENERATOR_AIN2          = 32;
const uint8_t TB_SMOKE_GENERATOR_AIN1          = 33;
const uint8_t TB_SMOKE_GENERATORFAN_STBY       = 34;
const uint8_t TB_SMOKE_FAN_IN1                 = 35;
const uint8_t TB_SMOKE_FAN_IN2                 = 36;
const uint8_t TB_SMOKE_FAN_PWM                 =  3;
const uint8_t TB_SMOKE_FAN_PWM_PRESCALER       =  1;

// Max, idle, moving voltage percents sent to smoke generator, suitable for fixed or proportional behaviour
const uint8_t SMOKE_GENERATOR_MAX_VOLTAGE_PERCENT    = 100;
const uint8_t SMOKE_GENERATOR_IDLE_VOLTAGE_PERCENT   =  50;
const uint8_t SMOKE_GENERATOR_MOVING_VOLTAGE_PERCENT = 100;

// Max, idle, moving voltage percents sent to smoke fan, suitable for fixed or proportional behaviour
const uint8_t SMOKE_FAN_MAX_VOLTAGE_PERCENT    = 100;
const uint8_t SMOKE_FAN_IDLE_VOLTAGE_PERCENT   =  50;
const uint8_t SMOKE_FAN_MOVING_VOLTAGE_PERCENT =  50;

// Smoke type: SmokeGeneratorBehaviour::proportional or SmokeGeneratorBehaviour::fixed
const SmokeGeneratorBehaviour SMOKE_BEHAVIOUR = SmokeGeneratorBehaviour::proportional;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Dasmikro TBS Mini sound unit
//--------------------------------------------------------------------------------------------------------------------------------------------------

// Dasmikro TBS Mini sound unit servo pins
const uint8_t TBSMINI_PROP1 = 41;
const uint8_t TBSMINI_PROP2 = 43;
const uint8_t TBSMINI_PROP3 = 45;

// Sounds asigned to PROP3 input (Coder 12-Key).
const uint8_t TBSMINI_SOUND_TURRET_ROTATION  =  1;
const uint8_t TBSMINI_SOUND_GUN_ELEVATION    =  2;
const uint8_t TBSMINI_SOUND_GUN_FIRE         =  3;
const uint8_t TBSMINI_SOUND_MACHINE_GUN_FIRE =  4;
const uint8_t TBSMINI_SOUND_VOLUME_UP        = 15;
const uint8_t TBSMINI_SOUND_VOLUME_DOWN      = 16;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Airsoft gun
//--------------------------------------------------------------------------------------------------------------------------------------------------

// Interrupt pin when airsoft mechanism has fired
const uint8_t AIRSOFT_FIRED_INTERRUPT_PIN = 18;

#endif

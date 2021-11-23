#ifndef CONFIG_H
#define CONFIG_H

#include <PS2X_lib.h>
#include "SmokeController.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------
// Arduino Panzer main properties
//--------------------------------------------------------------------------------------------------------------------------------------------------
const uint16_t AP_GUN_RELOAD_TIME = 2000;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// PS2 Radio
//--------------------------------------------------------------------------------------------------------------------------------------------------

// Miliseconds between reads
const uint8_t PS2_MS_BETWEEN_READS = 50;

// Milisecond to debounce buttons
const uint16_t PS2_MS_DEBOUNCE = 500;

// Stick properties
const int16_t PS2_MIN_STICK_VALUE    =   0;
const int16_t PS2_CENTER_STICK_VALUE = 128;
const int16_t PS2_MAX_STICK_VALUE    = 255;

// PS2 pins
const uint8_t PS2_DAT =   A2;
const uint8_t PS2_CMD =   A1;
const uint8_t PS2_SEL =   A0;
const uint8_t PS2_CLK =   A3;
const bool PS2_PRESSURES  = true;
const bool PS2_RUMBLE     = true;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// nRF24 Radio
//--------------------------------------------------------------------------------------------------------------------------------------------------

// Miliseconds between reads
const uint8_t NRF24_MS_BETWEEN_READS = 50;

// Max miliseconds between reads to detect lost connection
const uint16_t NRF24_MS_MAX_TIME_BETWEEN_READS = 1000;

// Stick properties
const int16_t NRF24_MIN_STICK_VALUE    =    0;
const int16_t NRF24_CENTER_STICK_VALUE =  512;
const int16_t NRF24_MAX_STICK_VALUE    = 1023;

// NRF24 module pins
const uint8_t NRF24_CE  = 49;
const uint8_t NRF24_CSN = 53;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Radio controller
//--------------------------------------------------------------------------------------------------------------------------------------------------

// Radio button abstraction
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
const uint16_t CONTROLLER_MS_DEBOUNCE = PS2_MS_DEBOUNCE;

// Selected radio type
#define RADIO_PS2_TYPE      1
#define RADIO_NRF24_TYPE    2

#define SELECTED_RADIO_TYPE RADIO_PS2_TYPE

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Drive direction
//--------------------------------------------------------------------------------------------------------------------------------------------------

// TB6612FNG Drive pins
const uint8_t  TB_DRIVE_APWM_PRESCALER =  1;
const uint8_t  TB_DRIVE_APWM           =  9;
const uint8_t  TB_DRIVE_AIN2           = 23;
const uint8_t  TB_DRIVE_AIN1           = 25;
const uint8_t  TB_DRIVE_STBY           = 27;
const uint8_t  TB_DRIVE_BIN1           = 29;
const uint8_t  TB_DRIVE_BIN2           = 31;
const uint8_t  TB_DRIVE_BPWM           = 10;
const uint16_t TB_DRIVE_BPWM_PRESCALER =  1;

// Max voltage percent sent to drive
const uint8_t DRIVE_MAX_VOLTAGE_PERCENT_ANALOG = 100;
const uint8_t DRIVE_MAX_VOLTAGE_PERCENT_PAD    =  80;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Turret rotation and gun elevation
//--------------------------------------------------------------------------------------------------------------------------------------------------

// TB6612FNG Turret rotation and gun elevation pins
const uint16_t TB_TURRETROTATION_PWM_PRESCALER =  1;
const uint8_t  TB_TURRETROTATION_PWM           = 11;
const uint8_t  TB_TURRETROTATION_IN2           = 22;
const uint8_t  TB_TURRETROTATION_IN1           = 24;
const uint8_t  TB_TURRETROTATION_STBY          = 26;

// Max voltage percent sent to rotation and elevation motors
const uint8_t TURRETROTARION_MAX_VOLTAGE_PERCENT = 100;
const uint8_t GUNELEVATION_MAX_VOLTAGE_PERCENT   = 100;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Smoke generator and fan
//--------------------------------------------------------------------------------------------------------------------------------------------------

// TB6612FNG Smoke generator and fan pins
const uint16_t TB_SMOKE_GENERATOR_PWM_PRESCALER =  1;
const uint8_t  TB_SMOKE_GENERATOR_PWM           =  2;
const uint8_t  TB_SMOKE_GENERATOR_AIN2          = 32;
const uint8_t  TB_SMOKE_GENERATOR_AIN1          = 33;
const uint8_t  TB_SMOKE_GENERATORFAN_STBY       = 34;
const uint8_t  TB_SMOKE_FAN_IN1                 = 35;
const uint8_t  TB_SMOKE_FAN_IN2                 = 36;
const uint8_t  TB_SMOKE_FAN_PWM                 =  3;
const uint16_t TB_SMOKE_FAN_PWM_PRESCALER       =  1;

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
const uint8_t TBSMINI_PROP1 = 37;
const uint8_t TBSMINI_PROP2 = 39;
const uint8_t TBSMINI_PROP3 = 41;

// Sounds asigned to PROP3 input (Coder 12-Key).
const uint8_t TBSMINI_SOUND_TURRET_ROTATION  =  1;
const uint8_t TBSMINI_SOUND_GUN_ELEVATION    =  2;
const uint8_t TBSMINI_SOUND_GUN_FIRE         =  3;
const uint8_t TBSMINI_SOUND_MACHINE_GUN_FIRE =  4;
const uint8_t TBSMINI_SOUND_VOLUME_UP        = 15;
const uint8_t TBSMINI_SOUND_VOLUME_DOWN      = 16;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Gun recoil
//--------------------------------------------------------------------------------------------------------------------------------------------------
const uint8_t  GUNRECOIL_SERVO_PIN      =  43;
const uint8_t  GUNRECOIL_DEGREES_IDLE   =   0;
const uint8_t  GUNRECOIL_DEGREES_RECOIL =  75;
const uint16_t GUNRECOIL_RECOIL_TIME   =  100;
const uint16_t GUNRECOIL_RETURN_TIME   = 1000;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Gun elevation
//--------------------------------------------------------------------------------------------------------------------------------------------------
const uint8_t GUNELEVATION_SERVO_PIN          =  45;
const int16_t GUNELEVATION_HORIZONTAL_DEGREES =  90;
const int16_t GUNELEVATION_DEPRESSION_DEGREES = -50;            // Respect to GUNELEVATION_HORIZONTAL_DEGREES
const int16_t GUNELEVATION_ELEVATION_DEGREES  =  50;            // Respect to GUNELEVATION_HORIZONTAL_DEGREES
const uint8_t GUNELEVATION_DEGREES_INCREMENT  =   5;
const int16_t GUNELEVATION_MS_INCREMENT       =  50;

#endif

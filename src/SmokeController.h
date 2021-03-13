#ifndef SMOKECONTROLLER_H
#define SMOKECONTROLLER_H

#include "DCMotorController.h"

class SmokeController {

    private:
        bool m_debugMode;                                                   // Debug mode
        byte m_maxGeneratorVoltagePercent, m_maxFanVoltagePercent;          // Max voltage applied from battery
        byte m_idleGeneratorVoltagePercent, m_idleFanVoltagePercent;        // Percent of the voltage in idle mode, respect to m_MaxVoltagePercent
        byte m_movingGeneratorVoltagePercent, m_movingFanVoltagePercent;    // Percent of the voltage while moving, respecto to m_MaxVoltagePercent
        byte m_pwmGeneratorPin, m_in1GeneratorPin, m_in2GeneratorPin;       // Generator pinout
        byte m_standbyPin;                                                  // Standby pin
        byte m_pwmFanPin, m_in1FanPin, m_in2FanPin;                         // Fan pinout
        byte m_idlePwmGenerator, m_idlePwmFan;                              // PWM output in idle mode
        byte m_movingPwmGenerator, m_movingPwmFan;                          // PWM output while moving

        void setGeneratorSpeed(byte pwmGenerator);
        void setFanSpeed(byte pwmFan);
        void disable();
        void printDebug(byte pwmGenerator, byte pwmFan);

    public:
        SmokeController();
        void begin(int pinSmokeGenerator[4], int pinSmokeFan[4]);
        void idle();
        void moving();
        void setGeneratorVoltagesPercent(byte maxGeneratorVoltagePercent, byte idleGeneratorVoltagePercent, byte movinGeneratorVoltagePercent);
        void setFanVoltagesPercent(byte maxFanVoltagePercent, byte idleFanVoltagePercent, byte movinFanVoltagePercent);
        void enableDebug();
        void disableDebug();
};

#endif

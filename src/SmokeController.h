#ifndef SMOKECONTROLLER_H
#define SMOKECONTROLLER_H

#include "DCMotorControllerProperties.h"

enum SmokeGeneratorBehaviour {
    proportional,
    fixed
};

class SmokeController {

    private:
        bool m_debugMode;                                                   // Debug mode
        bool m_isStarted;                                                   // is smoker started?
        SmokeGeneratorBehaviour m_behaviour;                                // Proportional smoke or fixed
        byte m_minProportional, m_maxProportional;                          // [m_minProportional, m_maxProportional] range for proportional smoke
        byte m_maxGeneratorVoltagePercent, m_maxFanVoltagePercent;          // Max voltage applied from battery
        byte m_idleGeneratorVoltagePercent, m_idleFanVoltagePercent;        // Percent of the voltage in idle mode, respect to m_MaxVoltagePercent
        byte m_movingGeneratorVoltagePercent, m_movingFanVoltagePercent;    // Percent of the voltage while moving, respecto to m_MaxVoltagePercent
        DCMotorControllerProperties m_smokerGeneratorProperties;            // Generator pinout
        DCMotorControllerProperties m_smokerFanProperties;                  // Fan pinout
        byte m_idlePwmFixedGenerator, m_idlePwmFixedFan;                    // Fixed PWM output in idle mode
        byte m_movingPwmFixedGenerator, m_movingPwmFixedFan;                // Fixed PWM output while moving

        void fixedSmoke();
        void proportionalSmoke(byte speed);
        void setGeneratorSpeed(byte pwmGenerator);
        void setFanSpeed(byte pwmFan);
        void disable();
        void printDebug(byte pwmGenerator, byte pwmFan);

    public:
        SmokeController();
        void begin(DCMotorControllerProperties smokerGeneratorProperties, DCMotorControllerProperties smokerFanProperties, SmokeGeneratorBehaviour behaviour);
        void toggle();
        void idle();
        void smoke(byte speed);
        void setProportional(byte minProportional, byte maxProportional);
        void setGeneratorVoltagesPercent(byte maxGeneratorVoltagePercent, byte idleGeneratorVoltagePercent, byte movinGeneratorVoltagePercent);
        void setFanVoltagesPercent(byte maxFanVoltagePercent, byte idleFanVoltagePercent, byte movinFanVoltagePercent);
        void enableDebug();
        void disableDebug();
};

#endif

#ifndef DCMOTORCONTROLLER_H
#define DCMOTORCONTROLLER_H

#include <Arduino.h>

typedef struct DCMotorControllerConfig {
    int pwmPrescaler;
    uint8_t pwmPin;
    uint8_t in1Pin;
    uint8_t in2Pin;
    uint8_t standbyPin;
};

class DCMotorController {

    private:
        bool m_debugMode;
        int m_stickMinValue;
        int m_stickMaxValue;
        int m_stickCenterValue;
        int m_pwmPrescaler;
        uint8_t m_pwmPin;
        uint8_t m_in1Pin;
        uint8_t m_in2Pin;
        uint8_t m_standbyPin;
        uint8_t m_maxVoltagePercent;
        String m_debugName;
        void printDEC(String label, int value);
        void setPWMPrescaler(uint8_t pin, uint16_t prescale);

    public:
        DCMotorController();
        void begin(DCMotorControllerConfig motorConfig, int stickProperties[3]);
        void move(int stickValue);
        void setMaxVoltagePercent(uint8_t percent);
        void enableMotor();
        void disableMotor();
        void enableDebug(String debugName);
        void disableDebug();

};

#endif

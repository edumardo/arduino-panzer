#ifndef DCMOTORCONTROLLER_H
#define DCMOTORCONTROLLER_H

class DCMotorController {

    private:
        bool m_debugMode;
        int m_stickMinValue;
        int m_stickMaxValue;
        int m_stickCenterValue;
        int m_pwmPin;
        int m_in1Pin;
        int m_in2Pin;
        int m_standbyPin;
        void print(int speed);

    public:
        DCMotorController();
        void begin(int pinMotor[4], int stickMinValue, int stickMaxValue, int stickCenterValue);
        void move(int stickValue);
        void enableMotor();
        void disableMotor();
        void enableDebug();
        void disableDebug();

};

#endif

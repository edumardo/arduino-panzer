#ifndef GUNRECOILPROPERTIES_H
#define GUNRECOILPROPERTIES_H

class GunRecoilProperties {
    public:
        uint8_t  servoPin;
        uint16_t mSRecoil;
        uint16_t mSReturn;
        int16_t  degreesIdle;
        int16_t  degreesRecoil;
};

#endif

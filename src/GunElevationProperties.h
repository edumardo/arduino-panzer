#ifndef GUNELEVATIONPROPERTIES_H
#define GUNELEVATIONPROPERTIES_H

class GunElevationProperties {
    public:
        uint8_t servoPin;
        int16_t depressionDegrees;
        int16_t elevationDegrees;
        int16_t horizontalDegrees;
        uint8_t degreesIncrement;
        int16_t mSIncrement;
};

#endif

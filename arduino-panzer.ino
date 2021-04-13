#include "src/ArduinoPanzer.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------
ArduinoPanzer tank;
Timer<> APTimer;

long debounceFireGunTime = 0;
long debounceStartStopTime = 0;
long debounceVolumeTime = 0;
//--------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {

    Serial.begin(115200);
    Serial.println("Inicializamos Serial");

    delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it

    tank.begin(&APTimer);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {

    if (tank.foundRadio() == false)
        return;

    APTimer.tick();

    tank.readRadio();

    /* Turret rotation and gun elevation */
    if ((tank.radioTurretRotation() != PS2_CENTER_STICK_VALUE) || (tank.radioGunElevation() != PS2_CENTER_STICK_VALUE)) {
        tank.turretRotation().move(tank.radioTurretRotation());
        tank.gunElevation().move(tank.radioGunElevation());
        tank.soundUnit()->playSound(TBSMINI_SOUND_TURRET_ROTATION);
    }
    else {
        tank.turretRotation().disableMotor();
        tank.gunElevation().disableMotor();
    }

    /* Analog stick driving */
    if(tank.radioSteeringThrottle() != PS2_CENTER_STICK_VALUE || tank.radioSteeringTurn() != PS2_CENTER_STICK_VALUE) {
        tank.driveDirection().move(tank.radioSteeringThrottle(), tank.radioSteeringTurn());
        int maxRadioSpeed = tank.maxRadioSpeed();
        tank.soundUnit()->setEngineSpeed(maxRadioSpeed);
        tank.smoker()->smoke(maxRadioSpeed);
    }
    /* Pad driving */
    else if (tank.radioButton(CONTROLLER_BUTTON_DRIVE_THROTTLE) || tank.radioButton(CONTROLLER_BUTTON_DRIVE_RIGHT) || tank.radioButton(CONTROLLER_BUTTON_DRIVE_LEFT) || tank.radioButton(CONTROLLER_BUTTON_DRIVE_REVERSE) ) {
        tank.driveDirection().move(tank.radioDriveThrottle(), tank.radioDriveReverse(), tank.radioDriveLeft(), tank.radioDriveRight());
        tank.soundUnit()->setEngineSpeed(90);        // [0, 127]
        tank.smoker()->smoke(CONTROLLER_BUTTON_DRIVE_THROTTLE);
    }
    else {
        tank.driveDirection().disableMotors();
        tank.soundUnit()->idleEngine();
        tank.smoker()->idle();
    }

    /* Fire gun */
    if ((tank.radioButton(CONTROLLER_FIRE_GUN)) && (millis() - debounceFireGunTime > CONTROLLER_MS_DEBOUNCE)) {
        debounceFireGunTime = millis();
        tank.soundUnit()->playSound(1);

    }

    /* Start/stop: engine and smoker */
    if ((tank.radioButton(CONTROLLER_STARTSTOP)) && (millis() - debounceStartStopTime > CONTROLLER_MS_DEBOUNCE)) {
        debounceStartStopTime = millis();
        tank.soundUnit()->toggleEngine();
        tank.smoker()->toggle();
    }

    /* Volume up/down */
    if ((tank.radioButton(CONTROLLER_VOLUMEUP)) && (millis() - debounceVolumeTime > CONTROLLER_MS_DEBOUNCE)) {
        debounceVolumeTime = millis();
        tank.soundUnit()->volumeUp();
    } else if ((tank.radioButton(CONTROLLER_VOLUMEDOWN)) && (millis() - debounceVolumeTime > CONTROLLER_MS_DEBOUNCE)) {
        debounceVolumeTime = millis();
        tank.soundUnit()->volumeDown();
    }

    delay(50);
}

#include <arduino-timer.h>
#include "src/main.h"
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

    tank.begin(&APTimer);
    tank.initTurretRotation();
    tank.initDriveDirection();
    tank.initAirsoftGun();
    tank.initSoundUnit();
    tank.initSmoker();

    delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
    tank.initController();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {

    if (tank.foundController() == false)
        return;

    APTimer.tick();

    tank.readController();

    /* Turret rotation and gun elevation */
    if ((tank.controllerTurretRotation() != CENTER_STICK_VALUE) || (tank.controllerGunElevation() != CENTER_STICK_VALUE)) {
        tank.turretRotation().move(tank.controllerTurretRotation());
        tank.gunElevation().move(tank.controllerGunElevation());
        tank.soundUnit()->playSound(TBSMINI_SOUND_TURRET_ROTATION);
    }
    else {
        tank.turretRotation().disableMotor();
        tank.gunElevation().disableMotor();
    }

    /* Analog stick driving */
    if(tank.controllerSteeringThrottle() != CENTER_STICK_VALUE || tank.controllerSteeringTurn() != CENTER_STICK_VALUE) {
        tank.driveDirection().move(tank.controllerSteeringThrottle(), tank.controllerSteeringTurn());
        byte maxControllerSpeed = tank.maxControllerSpeed();
        tank.soundUnit()->setEngineSpeed(maxControllerSpeed);
        tank.smoker()->smoke(maxControllerSpeed);
    }
    /* Pad driving */
    else if (tank.controllerButton(CONTROLLER_BUTTON_DRIVE_THROTTLE) || tank.controllerButton(CONTROLLER_BUTTON_DRIVE_RIGHT) || tank.controllerButton(CONTROLLER_BUTTON_DRIVE_LEFT) || tank.controllerButton(CONTROLLER_BUTTON_DRIVE_REVERSE) ) {
        tank.driveDirection().move(tank.controllerDriveThrottle(), tank.controllerDriveReverse(), tank.controllerDriveLeft(), tank.controllerDriveRight());
        tank.soundUnit()->setEngineSpeed(90);        // [0, 127]
        tank.smoker()->smoke(CONTROLLER_BUTTON_DRIVE_THROTTLE);
    }
    else {
        tank.driveDirection().disableMotors();
        tank.soundUnit()->idleEngine();
        tank.smoker()->idle();
    }

    /* Fire gun */
    if ((tank.controllerButton(CONTROLLER_FIRE_GUN)) && (millis() - debounceFireGunTime > CONTROLLER_MS_DEBOUNCE)) {
        debounceFireGunTime = millis();
        tank.soundUnit()->playSound(TBSMINI_SOUND_GUN_FIRE);

    }

    /* Start/stop: engine and smoker */
    if ((tank.controllerButton(CONTROLLER_STARTSTOP)) && (millis() - debounceStartStopTime > CONTROLLER_MS_DEBOUNCE)) {
        debounceStartStopTime = millis();
        tank.soundUnit()->toggleEngine();
        tank.smoker()->toggle();
    }

    /* Volume up/down */
    if ((tank.controllerButton(CONTROLLER_VOLUMEUP)) && (millis() - debounceVolumeTime > CONTROLLER_MS_DEBOUNCE)) {
        debounceVolumeTime = millis();
        tank.soundUnit()->volumeUp();
    } else if ((tank.controllerButton(CONTROLLER_VOLUMEDOWN)) && (millis() - debounceVolumeTime > CONTROLLER_MS_DEBOUNCE)) {
        debounceVolumeTime = millis();
        tank.soundUnit()->volumeDown();
    }

    delay(50);
}

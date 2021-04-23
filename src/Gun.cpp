#include "Gun.h"
#include "Utilities.h"

bool Gun::m_readyToFire;

Gun::Gun(){

    m_debugMode = false;
    m_readyToFire = true;
}

bool Gun::readyToFire() {
    return m_readyToFire;
}

void Gun::toggleDebugMode(bool debugMode) {

    m_debugMode = debugMode;
}

void Gun::printDebug() {

    printLabelBoolValue("readyToFire", m_readyToFire);
}

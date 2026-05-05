#include "RFReceiver.h"

RFReceiver::RFReceiver(RH_ASK* driver, Buzzer* buzzer)
: _driver(driver), _buzzer(buzzer) {}

void RFReceiver::begin() {}

void RFReceiver::update() {
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    // Borrar historial después de 5 segundos de inactividad
    if (_logs.size() > 0 && (millis() - _logTimestamp > 5000)) {
        _logs.clear();
    }

    if (_driver->recv(buf, &len)) {
        buf[len] = '\0'; // Asegurar fin de cadena
        String msg = String((char*)buf);
        
        Serial.print("RF Recibido: ");
        Serial.println(msg);

        addLog(msg);
        _buzzer->addTime(5000); // Activar buzzer 5 seg por cualquier RF
    }
}

void RFReceiver::addLog(String msg) {
    if (_logs.size() > 5) {
        _logs.erase(_logs.begin());
    }
    _logs.push_back(msg);
    _logTimestamp = millis();
}

int RFReceiver::getLogCount() {
    return _logs.size();
}

String RFReceiver::getLog(int index) {
    if (index >= 0 && (size_t)index < _logs.size()) {
        return _logs[index];
    }
    return "";
}

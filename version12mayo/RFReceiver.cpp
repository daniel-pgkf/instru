#include "RFReceiver.h"

RFReceiver::RFReceiver(RH_ASK* driver, Buzzer* buzzer)
: _driver(driver), _buzzer(buzzer) {}

void RFReceiver::begin() {}

void RFReceiver::update() {

    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    // limpiar logs después de 40 s
    if (_logs.size() > 0 && (millis() - _logTimestamp > 40000)) {
        _logs.clear();
    }

    if (_driver->recv(buf, &len)) {

        buf[len] = '\0';

        String msg = String((char*)buf);

        // validar formato RX_
        if (!msg.startsWith("RX_")) {
            return;
        }

        // obtener ID de alerta (A1, A2, etc)
        String alertID = msg.substring(3, 5);

        // anti broadcast storm
        if (alertID == _lastAlert) {
            return;
        }

        _lastAlert = alertID;

        // =========================
        // CREAR RETRANSMISION
        // =========================

        String retransmit = "RX_" + alertID + "_E9";

        // guardar retransmisión para webserver
        _lastRetransmission = retransmit;

        // enviar RF
        _driver->send(
            (uint8_t*)retransmit.c_str(),
            retransmit.length()
        );

        _driver->waitPacketSent();

        // guardar logs
        addLog("RX: " + msg);
        addLog("TX: " + retransmit);

        // buzzer
        _buzzer->addTime(10000);
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

String RFReceiver::getLastRetransmission() {
    return _lastRetransmission;
}
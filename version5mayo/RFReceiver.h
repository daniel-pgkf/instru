#pragma once

#include <RH_ASK.h>
#include "Buzzer.h"
#include <vector>

class RFReceiver {
public:
    RFReceiver(RH_ASK* driver, Buzzer* buzzer);

    void begin();
    void update();
    
    // Métodos para el WebServerManager
    int getLogCount();
    String getLog(int index);

private:
    RH_ASK* _driver;
    Buzzer* _buzzer;

    std::vector<String> _logs; 
    unsigned long _logTimestamp = 0;

    void addLog(String msg);
};

#pragma once
#include "../../models/include/devices/SmartDevice.hpp"
#include "../../models/include/EnvironmentSimulator.hpp"
#include <vector>
#include <memory>
#include <ctime>
using namespace std;

class AutomationService {
private:
    shared_ptr<EnvironmentSimulator> environmentSimulator;

public:
    AutomationService(shared_ptr<EnvironmentSimulator> simulator);
    
    void updateDevices(vector<shared_ptr<SmartDevice>>& devices);
    int getCurrentHour() const;
    
    double getCurrentTemperature() const;
    double getCurrentHumidity() const;
    double getCurrentCO2() const;
};


#pragma once
#include "SmartDevice.hpp"
using namespace std;

class SmartHeater : public SmartDevice {
private:
    double targetTemperature;
    double tolerance;

public:
    SmartHeater();
    SmartHeater(int deviceId, const string& name, const string& location,
                DeviceMode deviceMode, int power, double targetTemp, double deviceTolerance);
    
    double getTargetTemperature() const;
    void setTargetTemperature(double target);
    
    double getTolerance() const;
    void setTolerance(double deviceTolerance);
    
    void update(double temperature, double humidity, double co2, int hour) override;
    string getDeviceInfo() const override;
};


#pragma once
#include "SmartDevice.hpp"
using namespace std;

class SmartHumidifier : public SmartDevice {
private:
    double targetHumidity;
    double tolerance;

public:
    SmartHumidifier();
    SmartHumidifier(int deviceId, const string& name, const string& location,
                    DeviceMode deviceMode, int power, double targetHumidity, double deviceTolerance);
    
    double getTargetHumidity() const;
    void setTargetHumidity(double target);
    
    double getTolerance() const;
    void setTolerance(double deviceTolerance);
    
    void update(double temperature, double humidity, double co2, int hour) override;
    string getDeviceInfo() const override;
};


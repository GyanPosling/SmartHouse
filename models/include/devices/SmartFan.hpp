#pragma once
#include "SmartDevice.hpp"
using namespace std;

class SmartFan : public SmartDevice {
private:
    double targetCO2;
    double tolerance;

public:
    SmartFan();
    SmartFan(int deviceId, const string& name, const string& location,
             DeviceMode deviceMode, int power, double targetCO2, double deviceTolerance);
    
    double getTargetCO2() const;
    void setTargetCO2(double target);
    
    double getTolerance() const;
    void setTolerance(double deviceTolerance);
    
    // Перегрузка операторов - вызывают методы базового класса
    bool operator==(const SmartFan& other) const;
    bool operator<(const SmartFan& other) const;
    
    void update(double temperature, double humidity, double co2, int hour) override;
    string getDeviceInfo() const override;
};


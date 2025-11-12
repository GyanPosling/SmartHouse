#pragma once
#include "SmartDevice.hpp"
using namespace std;

class SmartLight : public SmartDevice {
private:
    int turnOffHour;

public:
    SmartLight();
    SmartLight(int deviceId, const string& name, const string& location,
               DeviceMode deviceMode, int power, int offHour);
    
    int getTurnOffHour() const;
    void setTurnOffHour(int hour);
    
    // Перегрузка операторов - вызывают методы базового класса
    bool operator==(const SmartLight& other) const;
    bool operator<(const SmartLight& other) const;
    
    void update(double temperature, double humidity, double co2, int hour) override;
    string getDeviceInfo() const override;
};


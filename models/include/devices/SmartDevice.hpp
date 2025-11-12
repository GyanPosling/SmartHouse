#pragma once
#include "Device.hpp"
using namespace std;

class SmartDevice : public Device {
protected:
    DeviceMode mode;

public:
    SmartDevice();
    SmartDevice(int deviceId, const string& name, const string& deviceLocation, 
                DeviceMode deviceMode, int power);
    SmartDevice(const SmartDevice& other);
    SmartDevice& operator=(const SmartDevice& other);
    virtual ~SmartDevice() = default;
    
    DeviceMode getMode() const;
    void setMode(DeviceMode deviceMode);
    string getModeString() const;
    
    // Перегрузка операторов сравнения - вызывают методы базового класса
    bool operator==(const SmartDevice& other) const;
    bool operator<(const SmartDevice& other) const;
    
    virtual void update(double temperature, double humidity, double co2, int hour) = 0;
    string getDeviceInfo() const override;
};


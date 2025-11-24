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
    
    bool operator==(const SmartDevice& other) const;
    bool operator<(const SmartDevice& other) const;
    
    friend ostream& operator<<(ostream& os, const SmartDevice& device);
    friend istream& operator>>(istream& is, SmartDevice& device);
    
    string getDeviceInfo() const override;
    
    void printHeader() const override;
    void printTable() const override;
    void updateField(int fieldChoice) override;
};
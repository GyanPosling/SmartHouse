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
    
    bool operator==(const SmartLight& other) const;
    bool operator<(const SmartLight& other) const;
    friend ostream& operator<<(ostream& os, const SmartLight& device);
    friend istream& operator>>(istream& is, SmartLight& device);
    
    string getDeviceInfo() const override;
    
    void printHeader() const override;
    void printTable() const override;
    void updateField(int fieldChoice) override;
};
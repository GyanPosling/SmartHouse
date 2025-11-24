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
    
    bool operator==(const SmartHeater& other) const;
    bool operator<(const SmartHeater& other) const;
    friend ostream& operator<<(ostream& os, const SmartHeater& device);
    friend istream& operator>>(istream& is, SmartHeater& device);
    
    string getDeviceInfo() const override;
    
    void printHeader() const override;
    void printTable() const override;
    void updateField(int fieldChoice) override;
};
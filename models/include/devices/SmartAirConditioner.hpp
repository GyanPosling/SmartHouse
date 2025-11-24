#pragma once
#include "SmartDevice.hpp"
using namespace std;

class SmartAirConditioner : public SmartDevice {
private:
    double targetCO2;
    double tolerance;

public:
    SmartAirConditioner();
    SmartAirConditioner(int deviceId, const string& name, const string& location,
                        DeviceMode deviceMode, int power, double targetCO2, double deviceTolerance);
    
    double getTargetCO2() const;
    void setTargetCO2(double target);
    
    double getTolerance() const;
    void setTolerance(double deviceTolerance);
    
    bool operator==(const SmartAirConditioner& other) const;
    bool operator<(const SmartAirConditioner& other) const;
    friend ostream& operator<<(ostream& os, const SmartAirConditioner& device);
    friend istream& operator>>(istream& is, SmartAirConditioner& device);
    
    string getDeviceInfo() const override;
    
    void printHeader() const override;
    void printTable() const override;
    void updateField(int fieldChoice) override;
};
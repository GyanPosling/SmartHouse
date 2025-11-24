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
    
    bool operator==(const SmartHumidifier& other) const;
    bool operator<(const SmartHumidifier& other) const;
    friend ostream& operator<<(ostream& os, const SmartHumidifier& device);
    friend istream& operator>>(istream& is, SmartHumidifier& device);
    
    string getDeviceInfo() const override;
    
    void printHeader() const override;
    void printTable() const override;
    void updateField(int fieldChoice) override;
};
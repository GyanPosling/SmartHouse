#pragma once
#include "ClimateDevice.hpp"

enum class TemperatureDeviceType {
    AIR_CONDITIONER,
    HEATER
};

class TemperatureDevice : public ClimateDevice {
private:
    TemperatureDeviceType deviceType;

public:
    TemperatureDevice();
    TemperatureDevice(int deviceId, const std::string& name, const std::string& location,
                     DeviceMode deviceMode, int power, double targetTemp, double tolerance,
                     TemperatureDeviceType type);
    
    TemperatureDeviceType getDeviceType() const;
    void setDeviceType(TemperatureDeviceType type);
    std::string getDeviceTypeString() const;
    
    void update(double temperature, double humidity) override;
    std::string getDeviceInfo() const override;
};

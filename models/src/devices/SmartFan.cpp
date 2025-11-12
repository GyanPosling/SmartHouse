#include "../../include/devices/SmartFan.hpp"

SmartFan::SmartFan() 
    : SmartDevice(), targetCO2(400.0), tolerance(50.0) {}

SmartFan::SmartFan(int deviceId, const string& name, const string& location,
                   DeviceMode deviceMode, int power, double targetCO2, double deviceTolerance)
    : SmartDevice(deviceId, name, location, deviceMode, power), 
      targetCO2(targetCO2), tolerance(deviceTolerance) {}

double SmartFan::getTargetCO2() const {
    return targetCO2;
}

void SmartFan::setTargetCO2(double target) {
    targetCO2 = target;
}

double SmartFan::getTolerance() const {
    return tolerance;
}

void SmartFan::setTolerance(double deviceTolerance) {
    tolerance = deviceTolerance;
}

void SmartFan::update(double temperature, double humidity, double co2, int hour) {
    if (mode == DeviceMode::MANUAL) {
        // В ручном режиме устройство работает пока пользователь не выключит
        return;
    }
    
    if (mode == DeviceMode::OFF) {
        turnOff();
        return;
    }
    
    // Автоматический режим - вентилятор работает для CO2
    if (co2 > targetCO2 + tolerance) {
        // CO2 выше нормы - включаем вентилятор
        turnOn();
    } else if (co2 <= targetCO2 - tolerance) {
        // CO2 в норме - выключаем
        turnOff();
    }
}

bool SmartFan::operator==(const SmartFan& other) const {
    return SmartDevice::operator==(other);
}

bool SmartFan::operator<(const SmartFan& other) const {
    return SmartDevice::operator<(other);
}

string SmartFan::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Fan" + 
           ", Target CO2: " + to_string(targetCO2) + " ppm" +
           ", Tolerance: ±" + to_string(tolerance) + " ppm";
}


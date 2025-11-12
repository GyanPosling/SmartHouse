#include "../../include/devices/SmartLight.hpp"

SmartLight::SmartLight() 
    : SmartDevice(), turnOffHour(22) {}

SmartLight::SmartLight(int deviceId, const string& name, const string& location,
                      DeviceMode deviceMode, int power, int offHour)
    : SmartDevice(deviceId, name, location, deviceMode, power), turnOffHour(offHour) {}

int SmartLight::getTurnOffHour() const {
    return turnOffHour;
}

void SmartLight::setTurnOffHour(int hour) {
    turnOffHour = hour;
}

void SmartLight::update(double temperature, double humidity, double co2, int hour) {
    if (mode == DeviceMode::MANUAL) {
        // В ручном режиме устройство работает пока пользователь не выключит
        return;
    }
    
    if (mode == DeviceMode::OFF) {
        turnOff();
        return;
    }
    
    // Автоматический режим - лампочка выключается после указанного часа
    if (hour >= turnOffHour) {
        // После 22:00 (или указанного часа) - выключаем
        turnOff();
    } else {
        // До указанного часа - включаем
        turnOn();
    }
}

bool SmartLight::operator==(const SmartLight& other) const {
    return SmartDevice::operator==(other);
}

bool SmartLight::operator<(const SmartLight& other) const {
    return SmartDevice::operator<(other);
}

string SmartLight::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Smart Light" + 
           ", Turn Off After: " + to_string(turnOffHour) + ":00";
}


#include "../../include/devices/SmartHumidifier.hpp"

SmartHumidifier::SmartHumidifier() 
    : SmartDevice(), targetHumidity(50.0), tolerance(5.0) {}

SmartHumidifier::SmartHumidifier(int deviceId, const string& name, const string& location,
                                DeviceMode deviceMode, int power, double targetHumidity, double deviceTolerance)
    : SmartDevice(deviceId, name, location, deviceMode, power), 
      targetHumidity(targetHumidity), tolerance(deviceTolerance) {}

double SmartHumidifier::getTargetHumidity() const {
    return targetHumidity;
}

void SmartHumidifier::setTargetHumidity(double target) {
    targetHumidity = target;
}

double SmartHumidifier::getTolerance() const {
    return tolerance;
}

void SmartHumidifier::setTolerance(double deviceTolerance) {
    tolerance = deviceTolerance;
}

void SmartHumidifier::update(double temperature, double humidity, double co2, int hour) {
    if (mode == DeviceMode::MANUAL) {
        // В ручном режиме устройство работает пока пользователь не выключит
        return;
    }
    
    if (mode == DeviceMode::OFF) {
        turnOff();
        return;
    }
    
    // Автоматический режим - увлажнитель работает для влажности
    if (humidity < targetHumidity - tolerance) {
        // Слишком сухо - включаем увлажнитель
        turnOn();
    } else if (humidity >= targetHumidity + tolerance) {
        // Влажность в норме или выше - выключаем
        turnOff();
    } else {
        // Влажность в норме - выключаем
        turnOff();
    }
}

string SmartHumidifier::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Humidifier" + 
           ", Target Humidity: " + to_string(targetHumidity) + "%" +
           ", Tolerance: ±" + to_string(tolerance) + "%";
}


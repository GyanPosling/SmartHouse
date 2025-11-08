#include "../../include/devices/SmartDehumidifier.hpp"

SmartDehumidifier::SmartDehumidifier() 
    : SmartDevice(), targetHumidity(50.0), tolerance(5.0) {}

SmartDehumidifier::SmartDehumidifier(int deviceId, const string& name, const string& location,
                                     DeviceMode deviceMode, int power, double targetHumidity, double deviceTolerance)
    : SmartDevice(deviceId, name, location, deviceMode, power), 
      targetHumidity(targetHumidity), tolerance(deviceTolerance) {}

double SmartDehumidifier::getTargetHumidity() const {
    return targetHumidity;
}

void SmartDehumidifier::setTargetHumidity(double target) {
    targetHumidity = target;
}

double SmartDehumidifier::getTolerance() const {
    return tolerance;
}

void SmartDehumidifier::setTolerance(double deviceTolerance) {
    tolerance = deviceTolerance;
}

void SmartDehumidifier::update(double temperature, double humidity, double co2, int hour) {
    if (mode == DeviceMode::MANUAL) {
        // В ручном режиме устройство работает пока пользователь не выключит
        return;
    }
    
    if (mode == DeviceMode::OFF) {
        turnOff();
        return;
    }
    
    // Автоматический режим - осушитель работает для влажности
    if (humidity > targetHumidity + tolerance) {
        // Слишком влажно - включаем осушитель
        turnOn();
    } else if (humidity <= targetHumidity - tolerance) {
        // Влажность в норме или ниже - выключаем
        turnOff();
    } else {
        // Влажность в норме - выключаем
        turnOff();
    }
}

string SmartDehumidifier::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Dehumidifier" + 
           ", Target Humidity: " + to_string(targetHumidity) + "%" +
           ", Tolerance: ±" + to_string(tolerance) + "%";
}


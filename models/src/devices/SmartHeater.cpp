#include "../../include/devices/SmartHeater.hpp"

SmartHeater::SmartHeater() 
    : SmartDevice(), targetTemperature(22.0), tolerance(2.0) {}

SmartHeater::SmartHeater(int deviceId, const string& name, const string& location,
                         DeviceMode deviceMode, int power, double targetTemp, double deviceTolerance)
    : SmartDevice(deviceId, name, location, deviceMode, power), 
      targetTemperature(targetTemp), tolerance(deviceTolerance) {}

double SmartHeater::getTargetTemperature() const {
    return targetTemperature;
}

void SmartHeater::setTargetTemperature(double target) {
    targetTemperature = target;
}

double SmartHeater::getTolerance() const {
    return tolerance;
}

void SmartHeater::setTolerance(double deviceTolerance) {
    tolerance = deviceTolerance;
}

void SmartHeater::update(double temperature, double humidity, double co2, int hour) {
    if (mode == DeviceMode::MANUAL) {
        // В ручном режиме устройство работает пока пользователь не выключит
        return;
    }
    
    if (mode == DeviceMode::OFF) {
        turnOff();
        return;
    }
    
    // Автоматический режим - обогреватель работает для температуры
    if (temperature < targetTemperature - tolerance) {
        // Холодно - включаем обогреватель
        turnOn();
    } else if (temperature >= targetTemperature + tolerance) {
        // Жарко - выключаем обогреватель
        turnOff();
    } else {
        // Температура в норме - выключаем
        turnOff();
    }
}

string SmartHeater::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Heater" + 
           ", Target Temperature: " + to_string(targetTemperature) + "°C" +
           ", Tolerance: ±" + to_string(tolerance) + "°C";
}


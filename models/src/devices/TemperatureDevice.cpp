#include "../../include/devices/TemperatureDevice.hpp"

TemperatureDevice::TemperatureDevice() 
    : ClimateDevice(), deviceType(TemperatureDeviceType::AIR_CONDITIONER) {}

TemperatureDevice::TemperatureDevice(int deviceId, const std::string& name, const std::string& location,
                                     DeviceMode deviceMode, int power, double targetTemp, double tolerance,
                                     TemperatureDeviceType type)
    : ClimateDevice(deviceId, name, location, deviceMode, power, targetTemp, tolerance), deviceType(type) {}

TemperatureDeviceType TemperatureDevice::getDeviceType() const {
    return deviceType;
}

void TemperatureDevice::setDeviceType(TemperatureDeviceType type) {
    deviceType = type;
}

std::string TemperatureDevice::getDeviceTypeString() const {
    switch (deviceType) {
        case TemperatureDeviceType::AIR_CONDITIONER:
            return "Кондиционер";
        case TemperatureDeviceType::HEATER:
            return "Обогреватель";
        default:
            return "Неизвестно";
    }
}

void TemperatureDevice::update(double temperature, double humidity) {
    if (mode == DeviceMode::MANUAL) {
        // В ручном режиме устройство работает пока пользователь не выключит
        return;
    }
    
    if (mode == DeviceMode::OFF) {
        turnOff();
        return;
    }
    
    // Автоматический режим
    if (isValueInRange(temperature)) {
        // Температура в норме - выключаем устройство
        turnOff();
    } else {
        // Температура не в норме
        if (deviceType == TemperatureDeviceType::HEATER) {
            // Обогреватель включается если холодно
            if (temperature < targetValue - tolerance) {
                turnOn();
            } else {
                turnOff();
            }
        } else if (deviceType == TemperatureDeviceType::AIR_CONDITIONER) {
            // Кондиционер включается если жарко
            if (temperature > targetValue + tolerance) {
                turnOn();
            } else {
                turnOff();
            }
        }
    }
}

std::string TemperatureDevice::getDeviceInfo() const {
    return Device::getDeviceInfo() + 
           ", Тип: " + getDeviceTypeString() + 
           ", Целевая температура: " + std::to_string(targetValue) + "°C" +
           ", Допуск: ±" + std::to_string(tolerance) + "°C";
}


#include "../../include/devices/SmartDevice.hpp"

SmartDevice::SmartDevice() 
    : Device(), mode(DeviceMode::AUTOMATIC) {}

SmartDevice::SmartDevice(int deviceId, const string& name, const string& deviceLocation,
                         DeviceMode deviceMode, int power)
    : Device(deviceId, name, deviceLocation, power), mode(deviceMode) {}

SmartDevice::SmartDevice(const SmartDevice& other)
    : Device(other), mode(other.mode) {}

SmartDevice& SmartDevice::operator=(const SmartDevice& other) {
    if (this != &other) {
        Device::operator=(other);
        mode = other.mode;
    }
    return *this;
}

DeviceMode SmartDevice::getMode() const {
    return mode;
}

void SmartDevice::setMode(DeviceMode deviceMode) {
    mode = deviceMode;
}

string SmartDevice::getModeString() const {
    switch (mode) {
        case DeviceMode::AUTOMATIC:
            return "Automatic";
        case DeviceMode::MANUAL:
            return "Manual";
        case DeviceMode::OFF:
            return "Off";
        default:
            return "Unknown";
    }
}

bool SmartDevice::operator==(const SmartDevice& other) const {
    // Если режим поиска - MODE, проверяем здесь
    if (Device::getSearchMode() == DeviceSearchField::MODE) {
        return mode == other.mode;
    }
    
    // Для остальных полей используем базовый класс
    return Device::operator==(other);
}

bool SmartDevice::operator<(const SmartDevice& other) const {
    // Если режим сортировки - MODE, проверяем здесь
    if (Device::getSearchMode() == DeviceSearchField::MODE) {
        return static_cast<int>(mode) < static_cast<int>(other.mode);
    }
    
    // Для остальных полей используем базовый класс
    return Device::operator<(other);
}

string SmartDevice::getDeviceInfo() const {
    return Device::getDeviceInfo() + ", Mode: " + getModeString();
}


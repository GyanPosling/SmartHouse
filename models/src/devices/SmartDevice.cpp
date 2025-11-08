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

string SmartDevice::getDeviceInfo() const {
    return Device::getDeviceInfo() + ", Mode: " + getModeString();
}


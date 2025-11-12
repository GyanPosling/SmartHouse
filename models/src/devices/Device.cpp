#include "../../include/devices/Device.hpp"
#include "../../exceptions/include/InputException.hpp"
#include <string>
#include <utility>
#include <sstream>
using namespace std;

// Инициализация статической переменной
DeviceSearchField Device::currentSearchMode = DeviceSearchField::ID;

Device::Device() 
    : id(0), deviceName(""), location(""), powerLevel(0), isOn(false) {}

Device::Device(int deviceId, const string& name, const string& deviceLocation, int power)
    : id(deviceId), deviceName(name), location(deviceLocation), powerLevel(power), isOn(false) {}

Device::Device(const Device& other)
    : id(other.id), deviceName(other.deviceName), location(other.location), 
      powerLevel(other.powerLevel), isOn(other.isOn) {}

Device& Device::operator=(const Device& other) {
    if (this != &other) {
        id = other.id;
        deviceName = other.deviceName;
        location = other.location;
        powerLevel = other.powerLevel;
        isOn = other.isOn;
    }
    return *this;
}

int Device::getId() const {
    return id;
}

void Device::setId(int deviceId) {
    id = deviceId;
}

string Device::getDeviceName() const {
    return deviceName;
}

void Device::setDeviceName(const string& name) {
    deviceName = name;
}

string Device::getLocation() const {
    return location;
}

void Device::setLocation(const string& deviceLocation) {
    location = deviceLocation;
}

int Device::getPowerLevel() const {
    return powerLevel;
}

void Device::setPowerLevel(int power) {
    powerLevel = power;
}

bool Device::getIsOn() const {
    return isOn;
}

void Device::setIsOn(bool on) {
    isOn = on;
}

void Device::turnOn() {
    isOn = true;
}

void Device::turnOff() {
    isOn = false;
}

string Device::getDeviceInfo() const {
    return "ID: " + to_string(id) + 
           ", Name: " + deviceName + 
           ", Location: " + location + 
           ", Power Level: " + to_string(powerLevel) + 
           ", On: " + (isOn ? "Yes" : "No");
}

void Device::setSearchMode(DeviceSearchField mode) {
    currentSearchMode = mode;
}

DeviceSearchField Device::getSearchMode() {
    return currentSearchMode;
}

bool Device::operator==(const Device& other) const {
    if (currentSearchMode == DeviceSearchField::ID) {
        return id == other.id;
    } else if (currentSearchMode == DeviceSearchField::DEVICE_NAME) {
        return deviceName == other.deviceName;
    } else if (currentSearchMode == DeviceSearchField::LOCATION) {
        return location == other.location;
    } else if (currentSearchMode == DeviceSearchField::POWER_LEVEL) {
        return powerLevel == other.powerLevel;
    }
    // Для MODE будет обработано в SmartDevice
    return id == other.id;
}

bool Device::operator!=(const Device& other) const {
    return !(*this == other);
}

bool Device::operator<(const Device& other) const {
    if (currentSearchMode == DeviceSearchField::ID) {
        return id < other.id;
    } else if (currentSearchMode == DeviceSearchField::DEVICE_NAME) {
        return deviceName < other.deviceName;
    } else if (currentSearchMode == DeviceSearchField::LOCATION) {
        return location < other.location;
    } else if (currentSearchMode == DeviceSearchField::POWER_LEVEL) {
        return powerLevel < other.powerLevel;
    }
    // Для MODE будет обработано в SmartDevice
    return id < other.id;
}

bool Device::operator>(const Device& other) const {
    return id > other.id;
}

bool Device::operator<=(const Device& other) const {
    return id <= other.id;
}

bool Device::operator>=(const Device& other) const {
    return id >= other.id;
}


ostream& operator<<(ostream& os, const Device& device) {
    os << device.id << "|" << device.deviceName << "|" << device.location 
       << "|" << device.powerLevel;
    return os;
}

istream& operator>>(istream& is, Device& device) {
    string line;
    if (getline(is, line)) {
        istringstream iss(line);
        string token;
        
        try {
            if (getline(iss, token, '|')) {
                device.id = stoi(token);
            }
            if (getline(iss, token, '|')) {
                device.deviceName = token;
            }
            if (getline(iss, token, '|')) {
                device.location = token;
            }
            if (getline(iss, token, '|')) {
                device.powerLevel = stoi(token);
            }
        } catch (const exception& e) {
            throw InputException(12, "Error reading device from stream: " + string(e.what()));
        }
    }
    return is;
}

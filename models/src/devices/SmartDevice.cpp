#include "../../include/devices/SmartDevice.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <iomanip>
using namespace std;

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

bool SmartDevice::operator==(const SmartDevice& other) const {
    if (Device::getSearchMode() == DeviceSearchField::MODE) {
        return mode == other.mode;
    }
    return static_cast<const Device&>(*this) == static_cast<const Device&>(other);
}

bool SmartDevice::operator<(const SmartDevice& other) const {
    if (Device::getSearchMode() == DeviceSearchField::MODE) {
        return static_cast<int>(mode) < static_cast<int>(other.mode);
    }
    return static_cast<const Device&>(*this) < static_cast<const Device&>(other);
}

ostream& operator<<(ostream& os, const SmartDevice& device) {
    os << static_cast<const Device&>(device) << "|" << static_cast<int>(device.mode);
    return os;
}

istream& operator>>(istream& is, SmartDevice& device) {
    is >> static_cast<Device&>(device);
    
    bool success = false;
    int modeInt = 0;
    while (!success) {
        try {
            safeInputInt(is, modeInt, 0, 2, "Enter mode (0-Automatic, 1-Manual, 2-Off): ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    device.mode = static_cast<DeviceMode>(modeInt);
    
    return is;
}

void SmartDevice::printHeader() const {
    cout << left;
    cout << "| " << setw(5) << "ID" << " | " << setw(15) << "Device Name" << " | " << setw(15) << "Location" << " | " << setw(12) << "Power Level" << " | " << setw(8) << "Status" << " | " << setw(10) << "Mode" << " |" << endl;
}

void SmartDevice::printTable() const {
    cout << left;
    cout << "| " << setw(5) << id << " | " << setw(15) << deviceName << " | " << setw(15) << location << " | " << setw(12) << powerLevel << " | " << setw(8) << (isOn ? "On" : "Off") << " | " << setw(10) << getModeString() << " |" << endl;
}

void SmartDevice::updateField(int fieldChoice) {
    string str;
    int num;
    if(fieldChoice <= 4) {
        Device::updateField(fieldChoice);
        return;
    }
    
    switch(fieldChoice) {
        case 5:
            safeInputInt(cin, num, 0, 2, "New mode (0-Automatic, 1-Manual, 2-Off): ");
            setMode(static_cast<DeviceMode>(num));
            break;
    }
}


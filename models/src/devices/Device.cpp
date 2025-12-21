#include "../../include/devices/Device.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <iomanip>
using namespace std;

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
    bool isCin = (&is == &cin);
    if (isCin) {
        bool success = false;
        while (!success) {
            try {
                device.id = safeInputNumeric<int>(is, 0, 999999, "Enter device ID: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        success = false;
        while (!success) {
            try {
                device.deviceName = safeGetLine(is, Language::ENGLISH, "Enter device name: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        success = false;
        while (!success) {
            try {
                device.location = safeGetLine(is, Language::ENGLISH, "Enter location: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        success = false;
        while (!success) {
            try {
                device.powerLevel = safeInputNumeric<int>(is, 0, 100, "Enter power level (0-100): ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
    } else {
        string line;
        if (getline(is, line)) {
            if (line.empty()) {
                is.setstate(ios::failbit);
                return is;
            }
            
            stringstream ss(line);
            string idStr, deviceNameStr, locationStr, powerLevelStr;
            
            if (getline(ss, idStr, '|') &&
                getline(ss, deviceNameStr, '|') &&
                getline(ss, locationStr, '|') &&
                getline(ss, powerLevelStr))
            {
                try {
                    device.id = stoi(idStr);
                    device.deviceName = deviceNameStr;
                    device.location = locationStr;
                    device.powerLevel = stoi(powerLevelStr);
                } catch (const exception&) {
                    is.setstate(ios::failbit);
                }
            } else {
                is.setstate(ios::failbit);
            }
        }
    }
    return is;
}

void Device::printHeader() const {
    cout << left;
    cout << "| " << setw(5) << "ID" << " | " << setw(15) << "Device Name" << " | " << setw(15) << "Location" << " | " << setw(12) << "Power Level" << " | " << setw(8) << "Status" << " |" << endl;
}

void Device::printTable() const {
    cout << left;
    cout << "| " << setw(5) << id << " | " << setw(15) << deviceName << " | " << setw(15) << location << " | " << setw(12) << powerLevel << " | " << setw(8) << (isOn ? "On" : "Off") << " |" << endl;
}

void Device::updateField(int fieldChoice) {
    bool success = false;
    while (!success) {
        try {
            switch(fieldChoice) {
                case 1:
                    id = safeInputNumeric<int>(cin, 0, 999999, "New device ID: ");
                    break;
                case 2:
                    deviceName = safeGetLine(cin, Language::ENGLISH, "New device name: ");
                    break;
                case 3:
                    location = safeGetLine(cin, Language::ENGLISH, "New location: ");
                    break;
                case 4:
                    powerLevel = safeInputNumeric<int>(cin, 0, 100, "New power level (0-100): ");
                    break;
                default:
                    // Р•СЃР»Рё РїРѕР»Рµ РЅРµ РЅР°Р№РґРµРЅРѕ, РІС‹С…РѕРґРёРј
                    return;
            }
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

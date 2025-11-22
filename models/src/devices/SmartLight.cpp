#include "../../include/devices/SmartLight.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <iomanip>
using namespace std;

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

ostream& operator<<(ostream& os, const SmartLight& device) {
    os << static_cast<const SmartDevice&>(device) << "|" << device.turnOffHour;
    return os;
}

istream& operator>>(istream& is, SmartLight& device) {
    is >> static_cast<SmartDevice&>(device);
    
    bool success = false;
    while (!success) {
        try {
            safeInputInt(is, device.turnOffHour, 0, 23, "Enter turn off hour (0-23): ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    return is;
}

void SmartLight::printHeader() const {
    cout << left;
    cout << "| " << setw(5) << "ID" << " | " << setw(15) << "Device Name" << " | " << setw(15) << "Location" << " | " << setw(12) << "Power Level" << " | " << setw(8) << "Status" << " | " << setw(10) << "Mode" << " | " << setw(15) << "Turn Off Hour" << " |" << endl;
}

void SmartLight::printTable() const {
    cout << left;
    cout << "| " << setw(5) << id << " | " << setw(15) << deviceName << " | " << setw(15) << location << " | " << setw(12) << powerLevel << " | " << setw(8) << (isOn ? "On" : "Off") << " | " << setw(10) << getModeString() << " | " << setw(15) << turnOffHour << " |" << endl;
}

void SmartLight::updateField(int fieldChoice) {
    int num;
    if(fieldChoice <= 5) {
        SmartDevice::updateField(fieldChoice);
        return;
    }
    
    switch(fieldChoice) {
        case 6:
            safeInputInt(cin, num, 0, 23, "New turn off hour (0-23): ");
            setTurnOffHour(num);
            break;
    }
}


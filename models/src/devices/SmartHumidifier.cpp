#include "../../include/devices/SmartHumidifier.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <iomanip>
using namespace std;

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



bool SmartHumidifier::operator==(const SmartHumidifier& other) const {
    return SmartDevice::operator==(other);
}

bool SmartHumidifier::operator<(const SmartHumidifier& other) const {
    return SmartDevice::operator<(other);
}

string SmartHumidifier::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Humidifier" + 
           ", Target Humidity: " + to_string(targetHumidity) + "%" +
           ", Tolerance: ±" + to_string(tolerance) + "%";
}

ostream& operator<<(ostream& os, const SmartHumidifier& device) {
    os << static_cast<const SmartDevice&>(device) << "|" << device.targetHumidity << "|" << device.tolerance;
    return os;
}

istream& operator>>(istream& is, SmartHumidifier& device) {
    is >> static_cast<SmartDevice&>(device);
    
    bool success = false;
    while (!success) {
        try {
            safeInputDouble(is, device.targetHumidity, 0, 100, "Enter target humidity: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    success = false;
    while (!success) {
        try {
            safeInputDouble(is, device.tolerance, 0, 50, "Enter tolerance: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    return is;
}

void SmartHumidifier::printHeader() const {
    cout << left;
    cout << "| " << setw(5) << "ID" << " | " << setw(15) << "Device Name" << " | " << setw(15) << "Location" << " | " << setw(12) << "Power Level" << " | " << setw(8) << "Status" << " | " << setw(10) << "Mode" << " | " << setw(15) << "Target Humidity" << " | " << setw(10) << "Tolerance" << " |" << endl;
}

void SmartHumidifier::printTable() const {
    cout << left;
    cout << "| " << setw(5) << id << " | " << setw(15) << deviceName << " | " << setw(15) << location << " | " << setw(12) << powerLevel << " | " << setw(8) << (isOn ? "On" : "Off") << " | " << setw(10) << getModeString() << " | " << setw(15) << targetHumidity << " | " << setw(10) << tolerance << " |" << endl;
}

void SmartHumidifier::updateField(int fieldChoice) {
    double num;
    if(fieldChoice <= 5) {
        SmartDevice::updateField(fieldChoice);
        return;
    }
    
    switch(fieldChoice) {
        case 6:
            safeInputDouble(cin, num, 0, 100, "New target humidity: ");
            setTargetHumidity(num);
            break;
        case 7:
            safeInputDouble(cin, num, 0, 50, "New tolerance: ");
            setTolerance(num);
            break;
    }
}


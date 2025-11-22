#include "../../include/devices/SmartFan.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <iomanip>
using namespace std;

SmartFan::SmartFan() 
    : SmartDevice(), targetCO2(400.0), tolerance(50.0) {}

SmartFan::SmartFan(int deviceId, const string& name, const string& location,
                   DeviceMode deviceMode, int power, double targetCO2, double deviceTolerance)
    : SmartDevice(deviceId, name, location, deviceMode, power), 
      targetCO2(targetCO2), tolerance(deviceTolerance) {}

double SmartFan::getTargetCO2() const {
    return targetCO2;
}

void SmartFan::setTargetCO2(double target) {
    targetCO2 = target;
}

double SmartFan::getTolerance() const {
    return tolerance;
}

void SmartFan::setTolerance(double deviceTolerance) {
    tolerance = deviceTolerance;
}



bool SmartFan::operator==(const SmartFan& other) const {
    return SmartDevice::operator==(other);
}

bool SmartFan::operator<(const SmartFan& other) const {
    return SmartDevice::operator<(other);
}

string SmartFan::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Fan" + 
           ", Target CO2: " + to_string(targetCO2) + " ppm" +
           ", Tolerance: ±" + to_string(tolerance) + " ppm";
}

ostream& operator<<(ostream& os, const SmartFan& device) {
    os << static_cast<const SmartDevice&>(device) << "|" << device.targetCO2 << "|" << device.tolerance;
    return os;
}

istream& operator>>(istream& is, SmartFan& device) {
    is >> static_cast<SmartDevice&>(device);
    
    bool success = false;
    while (!success) {
        try {
            safeInputDouble(is, device.targetCO2, 0, 10000, "Enter target CO2: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    success = false;
    while (!success) {
        try {
            safeInputDouble(is, device.tolerance, 0, 1000, "Enter tolerance: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    return is;
}

void SmartFan::printHeader() const {
    cout << left;
    cout << "| " << setw(5) << "ID" << " | " << setw(15) << "Device Name" << " | " << setw(15) << "Location" << " | " << setw(12) << "Power Level" << " | " << setw(8) << "Status" << " | " << setw(10) << "Mode" << " | " << setw(12) << "Target CO2" << " | " << setw(10) << "Tolerance" << " |" << endl;
}

void SmartFan::printTable() const {
    cout << left;
    cout << "| " << setw(5) << id << " | " << setw(15) << deviceName << " | " << setw(15) << location << " | " << setw(12) << powerLevel << " | " << setw(8) << (isOn ? "On" : "Off") << " | " << setw(10) << getModeString() << " | " << setw(12) << targetCO2 << " | " << setw(10) << tolerance << " |" << endl;
}

void SmartFan::updateField(int fieldChoice) {
    double num;
    if(fieldChoice <= 5) {
        SmartDevice::updateField(fieldChoice);
        return;
    }
    
    switch(fieldChoice) {
        case 6:
            safeInputDouble(cin, num, 0, 10000, "New target CO2: ");
            setTargetCO2(num);
            break;
        case 7:
            safeInputDouble(cin, num, 0, 1000, "New tolerance: ");
            setTolerance(num);
            break;
    }
}


#include "../../include/devices/SmartHeater.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <iomanip>
using namespace std;

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



bool SmartHeater::operator==(const SmartHeater& other) const {
    return SmartDevice::operator==(other);
}

bool SmartHeater::operator<(const SmartHeater& other) const {
    return SmartDevice::operator<(other);
}

string SmartHeater::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Heater" + 
           ", Target Temperature: " + to_string(targetTemperature) + "°C" +
           ", Tolerance: ±" + to_string(tolerance) + "°C";
}

ostream& operator<<(ostream& os, const SmartHeater& device) {
    os << static_cast<const SmartDevice&>(device) << "|" << device.targetTemperature << "|" << device.tolerance;
    return os;
}

istream& operator>>(istream& is, SmartHeater& device) {
    is >> static_cast<SmartDevice&>(device);
    
    bool success = false;
    while (!success) {
        try {
            safeInputDouble(is, device.targetTemperature, -50, 50, "Enter target temperature: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    success = false;
    while (!success) {
        try {
            safeInputDouble(is, device.tolerance, 0, 10, "Enter tolerance: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    return is;
}

void SmartHeater::printHeader() const {
    cout << left;
    cout << "| " << setw(5) << "ID" << " | " << setw(15) << "Device Name" << " | " << setw(15) << "Location" << " | " << setw(12) << "Power Level" << " | " << setw(8) << "Status" << " | " << setw(10) << "Mode" << " | " << setw(18) << "Target Temperature" << " | " << setw(10) << "Tolerance" << " |" << endl;
}

void SmartHeater::printTable() const {
    cout << left;
    cout << "| " << setw(5) << id << " | " << setw(15) << deviceName << " | " << setw(15) << location << " | " << setw(12) << powerLevel << " | " << setw(8) << (isOn ? "On" : "Off") << " | " << setw(10) << getModeString() << " | " << setw(18) << targetTemperature << " | " << setw(10) << tolerance << " |" << endl;
}

void SmartHeater::updateField(int fieldChoice) {
    double num;
    if(fieldChoice <= 5) {
        SmartDevice::updateField(fieldChoice);
        return;
    }
    
    switch(fieldChoice) {
        case 6:
            safeInputDouble(cin, num, -50, 50, "New target temperature: ");
            setTargetTemperature(num);
            break;
        case 7:
            safeInputDouble(cin, num, 0, 10, "New tolerance: ");
            setTolerance(num);
            break;
    }
}


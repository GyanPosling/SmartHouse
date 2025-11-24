#include "../../include/devices/SmartAirConditioner.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <iomanip>
using namespace std;

SmartAirConditioner::SmartAirConditioner() 
    : SmartDevice(), targetCO2(400.0), tolerance(50.0) {}

SmartAirConditioner::SmartAirConditioner(int deviceId, const string& name, const string& location,
                                         DeviceMode deviceMode, int power, double targetCO2, double deviceTolerance)
    : SmartDevice(deviceId, name, location, deviceMode, power), 
      targetCO2(targetCO2), tolerance(deviceTolerance) {}

double SmartAirConditioner::getTargetCO2() const {
    return targetCO2;
}

void SmartAirConditioner::setTargetCO2(double target) {
    targetCO2 = target;
}

double SmartAirConditioner::getTolerance() const {
    return tolerance;
}

void SmartAirConditioner::setTolerance(double deviceTolerance) {
    tolerance = deviceTolerance;
}

bool SmartAirConditioner::operator==(const SmartAirConditioner& other) const {
    return SmartDevice::operator==(other);
}

bool SmartAirConditioner::operator<(const SmartAirConditioner& other) const {
    return SmartDevice::operator<(other);
}

string SmartAirConditioner::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Air Conditioner" + 
           ", Target CO2: " + to_string(targetCO2) + " ppm" +
           ", Tolerance: ±" + to_string(tolerance) + " ppm";
}

ostream& operator<<(ostream& os, const SmartAirConditioner& device) {
    os << static_cast<const SmartDevice&>(device) << "|" << device.targetCO2 << "|" << device.tolerance;
    return os;
}

istream& operator>>(istream& is, SmartAirConditioner& device) {
    is >> static_cast<SmartDevice&>(device);
    
    bool isCin = (&is == &cin);
    if (isCin) {
        bool success = false;
        while (!success) {
            try {
                device.targetCO2 = safeInputNumeric<double>(is, 0, 10000, "Enter target CO2: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        success = false;
        while (!success) {
            try {
                device.tolerance = safeInputNumeric<double>(is, 0, 1000, "Enter tolerance: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
    } else {
        string line;
        if (getline(is, line)) {
            stringstream ss(line);
            string targetCO2Str, toleranceStr;
            if (getline(ss, targetCO2Str, '|') && getline(ss, toleranceStr)) {
                try {
                    device.targetCO2 = stod(targetCO2Str);
                    device.tolerance = stod(toleranceStr);
                } catch (const std::exception&) {
                    is.setstate(ios::failbit);
                }
            } else {
                is.setstate(ios::failbit);
            }
        }
    }
    
    return is;
}

void SmartAirConditioner::printHeader() const {
    cout << left;
    cout << "| " << setw(5) << "ID" << " | " << setw(15) << "Device Name" << " | " << setw(15) << "Location" << " | " << setw(12) << "Power Level" << " | " << setw(8) << "Status" << " | " << setw(10) << "Mode" << " | " << setw(12) << "Target CO2" << " | " << setw(10) << "Tolerance" << " |" << endl;
}

void SmartAirConditioner::printTable() const {
    cout << left;
    cout << "| " << setw(5) << id << " | " << setw(15) << deviceName << " | " << setw(15) << location << " | " << setw(12) << powerLevel << " | " << setw(8) << (isOn ? "On" : "Off") << " | " << setw(10) << getModeString() << " | " << setw(12) << targetCO2 << " | " << setw(10) << tolerance << " |" << endl;
}

void SmartAirConditioner::updateField(int fieldChoice) {
    if(fieldChoice <= 5) {
        SmartDevice::updateField(fieldChoice);
        return;
    }
    
    bool success = false;
    while (!success) {
        try {
            switch(fieldChoice) {
                case 6:
                    targetCO2 = safeInputNumeric<double>(cin, 0, 10000, "New target CO2: ");
                    break;
                case 7:
                    tolerance = safeInputNumeric<double>(cin, 0, 1000, "New tolerance: ");
                    break;
                default:
                    return;
            }
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}
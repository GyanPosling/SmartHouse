#include "../../include/devices/SmartDehumidifier.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <iomanip>
using namespace std;

SmartDehumidifier::SmartDehumidifier() 
    : SmartDevice(), targetHumidity(50.0), tolerance(5.0) {}

SmartDehumidifier::SmartDehumidifier(int deviceId, const string& name, const string& location,
                                     DeviceMode deviceMode, int power, double targetHumidity, double deviceTolerance)
    : SmartDevice(deviceId, name, location, deviceMode, power), 
      targetHumidity(targetHumidity), tolerance(deviceTolerance) {}

double SmartDehumidifier::getTargetHumidity() const {
    return targetHumidity;
}

void SmartDehumidifier::setTargetHumidity(double target) {
    targetHumidity = target;
}

double SmartDehumidifier::getTolerance() const {
    return tolerance;
}

void SmartDehumidifier::setTolerance(double deviceTolerance) {
    tolerance = deviceTolerance;
}


bool SmartDehumidifier::operator==(const SmartDehumidifier& other) const {
    return SmartDevice::operator==(other);
}

bool SmartDehumidifier::operator<(const SmartDehumidifier& other) const {
    return SmartDevice::operator<(other);
}

string SmartDehumidifier::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Dehumidifier" + 
           ", Target Humidity: " + to_string(targetHumidity) + "%" +
           ", Tolerance: ±" + to_string(tolerance) + "%";
}

ostream& operator<<(ostream& os, const SmartDehumidifier& device) {
    os << static_cast<const SmartDevice&>(device) << "|" << device.targetHumidity << "|" << device.tolerance;
    return os;
}

istream& operator>>(istream& is, SmartDehumidifier& device) {
    is >> static_cast<SmartDevice&>(device);
    
    bool isCin = (&is == &cin);
    if (isCin) {
        bool success = false;
        while (!success) {
            try {
                device.targetHumidity = safeInputNumeric<double>(is, 0, 100, "Enter target humidity: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        success = false;
        while (!success) {
            try {
                device.tolerance = safeInputNumeric<double>(is, 0, 50, "Enter tolerance: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
    } else {
        string line;
        if (getline(is, line)) {
            stringstream ss(line);
            string targetHumidityStr, toleranceStr;
            if (getline(ss, targetHumidityStr, '|') && getline(ss, toleranceStr)) {
                try {
                    device.targetHumidity = stod(targetHumidityStr);
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

void SmartDehumidifier::printHeader() const {
    cout << left;
    cout << "| " << setw(5) << "ID" << " | " << setw(15) << "Device Name" << " | " << setw(15) << "Location" << " | " << setw(12) << "Power Level" << " | " << setw(8) << "Status" << " | " << setw(10) << "Mode" << " | " << setw(15) << "Target Humidity" << " | " << setw(10) << "Tolerance" << " |" << endl;
}

void SmartDehumidifier::printTable() const {
    cout << left;
    cout << "| " << setw(5) << id << " | " << setw(15) << deviceName << " | " << setw(15) << location << " | " << setw(12) << powerLevel << " | " << setw(8) << (isOn ? "On" : "Off") << " | " << setw(10) << getModeString() << " | " << setw(15) << targetHumidity << " | " << setw(10) << tolerance << " |" << endl;
}

void SmartDehumidifier::updateField(int fieldChoice) {
    if(fieldChoice <= 5) {
        SmartDevice::updateField(fieldChoice);
        return;
    }
    
    bool success = false;
    while (!success) {
        try {
            switch(fieldChoice) {
                case 6:
                    targetHumidity = safeInputNumeric<double>(cin, 0, 100, "New target humidity: ");
                    break;
                case 7:
                    tolerance = safeInputNumeric<double>(cin, 0, 50, "New tolerance: ");
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
#include "../include/DeviceService.hpp"
#include "../exceptions/include/FileException.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>

DeviceService::DeviceService() 
    : filename("devices.txt") {
    loadAllDevices();
}

void DeviceService::addDevice(shared_ptr<SmartDevice> device) {
    if (device == nullptr) {
        throw FileException(100, "Attempt to add empty device");
    }
    devices.push_back(device);
    saveAllDevices();
}

void DeviceService::removeDevice(int deviceId) {
    auto it = remove_if(devices.begin(), devices.end(),
        [deviceId](const shared_ptr<SmartDevice>& d) {
            return d->getId() == deviceId;
        });
    
    if (it == devices.end()) {
        throw FileException(101, "Device with ID " + to_string(deviceId) + " not found");
    }
    
    devices.erase(it, devices.end());
    saveAllDevices();
}

void DeviceService::updateDevice(int deviceId, shared_ptr<SmartDevice> updatedDevice) {
    bool found = false;
    for (size_t i = 0; i < devices.size(); ++i) {
        if (devices[i]->getId() == deviceId) {
            devices[i] = updatedDevice;
            found = true;
            break;
        }
    }
    
    if (!found) {
        throw FileException(103, "Device with ID " + to_string(deviceId) + " not found");
    }
    
    saveAllDevices();
}

shared_ptr<SmartDevice> DeviceService::getDeviceById(int deviceId) {
    for (const auto& device : devices) {
        if (device->getId() == deviceId) {
            return device;
        }
    }
    return nullptr;
}

vector<shared_ptr<SmartDevice>> DeviceService::getAllDevices() {
    return devices;
}

vector<shared_ptr<SmartDevice>> DeviceService::searchDevices(DeviceSearchField field, const string& value) {
    Device::setSearchMode(field);
    vector<shared_ptr<SmartDevice>> results;
    
    for (const auto& device : devices) {
        if (field == DeviceSearchField::ID) {
            if (to_string(device->getId()) == value) {
                results.push_back(device);
            }
        } else if (field == DeviceSearchField::DEVICE_NAME) {
            if (device->getDeviceName() == value) {
                results.push_back(device);
            }
        } else if (field == DeviceSearchField::LOCATION) {
            if (device->getLocation() == value) {
                results.push_back(device);
            }
        } else if (field == DeviceSearchField::POWER_LEVEL) {
            if (to_string(device->getPowerLevel()) == value) {
                results.push_back(device);
            }
        } else if (field == DeviceSearchField::MODE) {
            SmartDevice* smartDevice = dynamic_cast<SmartDevice*>(device.get());
            if (smartDevice && smartDevice->getModeString() == value) {
                results.push_back(device);
            }
        }
    }
    
    return results;
}

void DeviceService::sortDevices(DeviceSortField field) {
    DeviceSearchField searchField;
    switch (field) {
        case DeviceSortField::ID:
            searchField = DeviceSearchField::ID;
            break;
        case DeviceSortField::DEVICE_NAME:
            searchField = DeviceSearchField::DEVICE_NAME;
            break;
        case DeviceSortField::LOCATION:
            searchField = DeviceSearchField::LOCATION;
            break;
        case DeviceSortField::MODE:
            searchField = DeviceSearchField::MODE;
            break;
        case DeviceSortField::POWER_LEVEL:
            searchField = DeviceSearchField::POWER_LEVEL;
            break;
        default:
            searchField = DeviceSearchField::ID;
    }
    
    Device::setSearchMode(searchField);
    
    sort(devices.begin(), devices.end(),
        [](const shared_ptr<SmartDevice>& a, const shared_ptr<SmartDevice>& b) {
            return *a < *b;
        });
}

void DeviceService::saveAllDevices() {
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) {
        throw FileException(100, "Failed to open file " + filename + " for writing");
    }
    
    for (const auto& device : devices) {
        string deviceType;
        if (dynamic_cast<SmartAirConditioner*>(device.get())) {
            deviceType = "AirConditioner";
            file << deviceType << "|" << *dynamic_cast<SmartAirConditioner*>(device.get()) << "\n";
        } else if (dynamic_cast<SmartHeater*>(device.get())) {
            deviceType = "Heater";
            file << deviceType << "|" << *dynamic_cast<SmartHeater*>(device.get()) << "\n";
        } else if (dynamic_cast<SmartHumidifier*>(device.get())) {
            deviceType = "Humidifier";
            file << deviceType << "|" << *dynamic_cast<SmartHumidifier*>(device.get()) << "\n";
        } else if (dynamic_cast<SmartDehumidifier*>(device.get())) {
            deviceType = "Dehumidifier";
            file << deviceType << "|" << *dynamic_cast<SmartDehumidifier*>(device.get()) << "\n";
        } else if (dynamic_cast<SmartFan*>(device.get())) {
            deviceType = "Fan";
            file << deviceType << "|" << *dynamic_cast<SmartFan*>(device.get()) << "\n";
        } else if (dynamic_cast<SmartLight*>(device.get())) {
            deviceType = "Light";
            file << deviceType << "|" << *dynamic_cast<SmartLight*>(device.get()) << "\n";
        }
    }
    
    file.close();
}

void DeviceService::loadAllDevices() {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        size_t pos = line.find('|');
        if (pos == string::npos) continue;
        
        string deviceType = line.substr(0, pos);
        string deviceData = line.substr(pos + 1);
        
        istringstream iss(deviceData);
        string token;
        vector<string> tokens;
        
        while (getline(iss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() < 5) continue;
        
        shared_ptr<SmartDevice> device;
        
        try {
            int id = stoi(tokens[0]);
            string name = tokens[1];
            string location = tokens[2];
            int powerLevel = stoi(tokens[3]);
            DeviceMode mode = static_cast<DeviceMode>(stoi(tokens[4]));
            
            if (deviceType == "AirConditioner" && tokens.size() >= 7) {
                double targetCO2 = stod(tokens[5]);
                double tolerance = stod(tokens[6]);
                device = make_shared<SmartAirConditioner>(id, name, location, mode, powerLevel, targetCO2, tolerance);
            } else if (deviceType == "Heater" && tokens.size() >= 7) {
                double targetTemp = stod(tokens[5]);
                double tolerance = stod(tokens[6]);
                device = make_shared<SmartHeater>(id, name, location, mode, powerLevel, targetTemp, tolerance);
            } else if (deviceType == "Humidifier" && tokens.size() >= 7) {
                double targetHumidity = stod(tokens[5]);
                double tolerance = stod(tokens[6]);
                device = make_shared<SmartHumidifier>(id, name, location, mode, powerLevel, targetHumidity, tolerance);
            } else if (deviceType == "Dehumidifier" && tokens.size() >= 7) {
                double targetHumidity = stod(tokens[5]);
                double tolerance = stod(tokens[6]);
                device = make_shared<SmartDehumidifier>(id, name, location, mode, powerLevel, targetHumidity, tolerance);
            } else if (deviceType == "Fan" && tokens.size() >= 7) {
                double targetCO2 = stod(tokens[5]);
                double tolerance = stod(tokens[6]);
                device = make_shared<SmartFan>(id, name, location, mode, powerLevel, targetCO2, tolerance);
            } else if (deviceType == "Light" && tokens.size() >= 6) {
                int turnOffHour = stoi(tokens[5]);
                device = make_shared<SmartLight>(id, name, location, mode, powerLevel, turnOffHour);
            }
        } catch (...) {
            continue;
        }
        
        if (device) {
            devices.push_back(device);
        }
    }
    
    file.close();
}

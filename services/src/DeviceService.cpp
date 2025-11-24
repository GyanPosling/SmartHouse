#include "../include/DeviceService.hpp"
#include "../exceptions/include/FileException.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>

DeviceService::DeviceService() 
    : deviceFile("devices.txt") {
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

const vector<shared_ptr<SmartDevice>> DeviceService::getAllDevices() const{
    return devices;
}

vector<shared_ptr<SmartDevice>> DeviceService::searchDevices(int option) {
    string searchString;
    int searchInt = 0;
    
    shared_ptr<SmartDevice> searchObj = make_shared<SmartDevice>();
    
    if (option == 1) {
        cout << "Enter ID to search: ";
        cin >> searchInt;
        searchObj->setId(searchInt);
        Device::setSearchMode(DeviceSearchField::ID);
    } else if (option == 2) {
        cout << "Enter device name to search: ";
        cin >> searchString;
        searchObj->setDeviceName(searchString);
        Device::setSearchMode(DeviceSearchField::DEVICE_NAME);
    } else if (option == 3) {
        cout << "Enter location to search: ";
        cin >> searchString;
        searchObj->setLocation(searchString);
        Device::setSearchMode(DeviceSearchField::LOCATION);
    } else if (option == 4) {
        cout << "Enter power level to search: ";
        cin >> searchInt;
        searchObj->setPowerLevel(searchInt);
        Device::setSearchMode(DeviceSearchField::POWER_LEVEL);
    } else if (option == 5) {
        cout << "Enter mode to search (0-Automatic, 1-Manual, 2-Off): ";
        cin >> searchInt;
        searchObj->setMode(static_cast<DeviceMode>(searchInt));
        Device::setSearchMode(DeviceSearchField::MODE);
    }
    
    vector<shared_ptr<SmartDevice>> searchResults;
    
    for (const auto& device : devices) {
        if (*device == *searchObj) {
            searchResults.push_back(device);
        }
    }
    
    Device::setSearchMode(DeviceSearchField::ID);
    
    return searchResults;
}

void DeviceService::sortDevices(int option) {
    if (devices.empty()) {
        cout << "No devices to sort." << endl;
        return;
    }
    
    switch (option) {
        case 1:
            Device::setSearchMode(DeviceSearchField::ID);
            cout << "Sorting by ID..." << endl;
            break;
        case 2:
            Device::setSearchMode(DeviceSearchField::DEVICE_NAME);
            cout << "Sorting by Device Name..." << endl;
            break;
        case 3:
            Device::setSearchMode(DeviceSearchField::LOCATION);
            cout << "Sorting by Location..." << endl;
            break;
        case 4:
            Device::setSearchMode(DeviceSearchField::POWER_LEVEL);
            cout << "Sorting by Power Level..." << endl;
            break;
        case 5:
            Device::setSearchMode(DeviceSearchField::MODE);
            cout << "Sorting by Mode..." << endl;
            break;
        default:
            Device::setSearchMode(DeviceSearchField::ID);
            cout << "Sorting by ID..." << endl;
            break;
    }
    
    sort(devices.begin(), devices.end(),
        [](const shared_ptr<SmartDevice>& a, const shared_ptr<SmartDevice>& b) {
            return *a < *b;
        });
    
    Device::setSearchMode(DeviceSearchField::ID);
    
    cout << "Devices sorted successfully." << endl;
}

void DeviceService::saveAllDevices() {
    deviceFile.clearFile();
    
    for (const auto& device : devices) {
        ostringstream oss;
        string deviceType;
        
        if (dynamic_cast<SmartAirConditioner*>(device.get())) {
            deviceType = "AirConditioner";
            oss << *dynamic_cast<SmartAirConditioner*>(device.get());
        } else if (dynamic_cast<SmartHeater*>(device.get())) {
            deviceType = "Heater";
            oss << *dynamic_cast<SmartHeater*>(device.get());
        } else if (dynamic_cast<SmartHumidifier*>(device.get())) {
            deviceType = "Humidifier";
            oss << *dynamic_cast<SmartHumidifier*>(device.get());
        } else if (dynamic_cast<SmartDehumidifier*>(device.get())) {
            deviceType = "Dehumidifier";
            oss << *dynamic_cast<SmartDehumidifier*>(device.get());
        } else if (dynamic_cast<SmartFan*>(device.get())) {
            deviceType = "Fan";
            oss << *dynamic_cast<SmartFan*>(device.get());
        } else if (dynamic_cast<SmartLight*>(device.get())) {
            deviceType = "Light";
            oss << *dynamic_cast<SmartLight*>(device.get());
        }
        
        if (!deviceType.empty()) {
            string deviceData = deviceType + "|" + oss.str();
            deviceFile.saveRecord(deviceData);
        }
    }
}

void DeviceService::loadAllDevices() {
    auto records = deviceFile.readAllRecords();
    
    for (const auto& record : records) {
        if (record->empty()) continue;
        
        string line = *record;
        size_t pos = line.find('|');
        if (pos == string::npos) continue;
        
        string deviceType = line.substr(0, pos);
        string deviceData = line.substr(pos + 1);
        
        shared_ptr<SmartDevice> device;
        
        try {
            if (deviceType == "AirConditioner") {
                auto ac = make_shared<SmartAirConditioner>();
                istringstream iss(deviceData);
                iss >> *ac;
                device = ac;
            } else if (deviceType == "Heater") {
                auto heater = make_shared<SmartHeater>();
                istringstream iss(deviceData);
                iss >> *heater;
                device = heater;
            } else if (deviceType == "Humidifier") {
                auto humidifier = make_shared<SmartHumidifier>();
                istringstream iss(deviceData);
                iss >> *humidifier;
                device = humidifier;
            } else if (deviceType == "Dehumidifier") {
                auto dehumidifier = make_shared<SmartDehumidifier>();
                istringstream iss(deviceData);
                iss >> *dehumidifier;
                device = dehumidifier;
            } else if (deviceType == "Fan") {
                auto fan = make_shared<SmartFan>();
                istringstream iss(deviceData);
                iss >> *fan;
                device = fan;
            } else if (deviceType == "Light") {
                auto light = make_shared<SmartLight>();
                istringstream iss(deviceData);
                iss >> *light;
                device = light;
            }
        } catch (...) {
            continue;
        }
        
        if (device) {
            devices.push_back(device);
        }
        delete record;
    }
}
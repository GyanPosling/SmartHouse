#include "../include/DeviceService.hpp"
#include "../exceptions/include/FileException.hpp"
#include <fstream>
#include <sstream>

DeviceService::DeviceService() 
    : deviceFile("devices.txt") {
    loadAllDevices();
}

DeviceService::~DeviceService() {
    deleteAll();
}

void DeviceService::deleteAll() {
    while (!devices.isEmpty()) {
        SmartDevice* toDelete = devices.popFront();
        delete toDelete;
    }
}

void DeviceService::addDevice(SmartDevice* device) {
    if (device == nullptr) {
        throw FileException(100, "Attempt to add empty device");
    }
    devices.pushBack(device);
    saveAllDevices();
}

void DeviceService::removeDevice(int deviceId) {
    bool found = false;
    for (auto it = devices.begin(); it != devices.end(); ++it) {
        SmartDevice* current = *it;
        if (current->getId() == deviceId) {
            delete current;
            devices.removeAt(it);
            found = true;
            break;
        }
    }
    
    if (!found) {
        throw FileException(101, "Device with ID " + to_string(deviceId) + " not found");
    }
    
    saveAllDevices();
}

void DeviceService::updateDevice(int deviceId, SmartDevice* updatedDevice) {
    bool found = false;
    for (auto it = devices.begin(); it != devices.end(); ++it) {
        SmartDevice* current = *it;
        if (current->getId() == deviceId) {
            delete current;
            *it = updatedDevice;
            found = true;
            break;
        }
    }
    
    if (!found) {
        delete updatedDevice;
        throw FileException(103, "Device with ID " + to_string(deviceId) + " not found");
    }
    
    saveAllDevices();
}

SmartDevice* DeviceService::getDeviceById(int deviceId) {
    for (const auto& device : devices) {
        if (device->getId() == deviceId) {
            return device;
        }
    }
    return nullptr;
}

Deque<SmartDevice*> DeviceService::getAllDevices() const{
    return devices;
}

Deque<SmartDevice*> DeviceService::searchDevices(int option) {
    string searchString;
    int searchInt = 0;
    
    SmartDevice searchObj;
    
    if (option == 1) {
        cout << "Enter ID to search: ";
        cin >> searchInt;
        searchObj.setId(searchInt);
        Device::setSearchMode(DeviceSearchField::ID);
    } else if (option == 2) {
        cout << "Enter device name to search: ";
        cin >> searchString;
        searchObj.setDeviceName(searchString);
        Device::setSearchMode(DeviceSearchField::DEVICE_NAME);
    } else if (option == 3) {
        cout << "Enter location to search: ";
        cin >> searchString;
        searchObj.setLocation(searchString);
        Device::setSearchMode(DeviceSearchField::LOCATION);
    } else if (option == 4) {
        cout << "Enter power level to search: ";
        cin >> searchInt;
        searchObj.setPowerLevel(searchInt);
        Device::setSearchMode(DeviceSearchField::POWER_LEVEL);
    } else if (option == 5) {
        cout << "Enter mode to search (0-Automatic, 1-Manual, 2-Off): ";
        cin >> searchInt;
        searchObj.setMode(static_cast<DeviceMode>(searchInt));
        Device::setSearchMode(DeviceSearchField::MODE);
    }
    
    Deque<SmartDevice*> searchResults;
    
    for (const auto& device : devices) {
        if (*device == searchObj) {
            searchResults.pushBack(device);
        }
    }
    
    Device::setSearchMode(DeviceSearchField::ID);
    
    return searchResults;
}

void DeviceService::sortDevices(int option) {
    if (devices.isEmpty()) {
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
    
    devices.sortDeque([](SmartDevice* a, SmartDevice* b) {
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
        
        if (dynamic_cast<SmartAirConditioner*>(device)) {
            deviceType = "AirConditioner";
            oss << *dynamic_cast<SmartAirConditioner*>(device);
        } else if (dynamic_cast<SmartHeater*>(device)) {
            deviceType = "Heater";
            oss << *dynamic_cast<SmartHeater*>(device);
        } else if (dynamic_cast<SmartHumidifier*>(device)) {
            deviceType = "Humidifier";
            oss << *dynamic_cast<SmartHumidifier*>(device);
        } else if (dynamic_cast<SmartDehumidifier*>(device)) {
            deviceType = "Dehumidifier";
            oss << *dynamic_cast<SmartDehumidifier*>(device);
        } else if (dynamic_cast<SmartFan*>(device)) {
            deviceType = "Fan";
            oss << *dynamic_cast<SmartFan*>(device);
        } else if (dynamic_cast<SmartLight*>(device)) {
            deviceType = "Light";
            oss << *dynamic_cast<SmartLight*>(device);
        }
        
        if (!deviceType.empty()) {
            string deviceData = deviceType + "|" + oss.str();
            deviceFile.saveRecord(deviceData);
        }
    }
}

void DeviceService::loadAllDevices() {
    deleteAll();
    auto records = deviceFile.readAllRecords();
    
    for (const auto& record : records) {
        if (record->empty()) {
            delete record;
            continue;
        }
        
        string line = *record;
        size_t pos = line.find('|');
        if (pos == string::npos) {
            delete record;
            continue;
        }
        
        string deviceType = line.substr(0, pos);
        string deviceData = line.substr(pos + 1);
        
        SmartDevice* device = nullptr;
        
        try {
            if (deviceType == "AirConditioner") {
                auto ac = new SmartAirConditioner();
                istringstream iss(deviceData);
                iss >> *ac;
                device = ac;
            } else if (deviceType == "Heater") {
                auto heater = new SmartHeater();
                istringstream iss(deviceData);
                iss >> *heater;
                device = heater;
            } else if (deviceType == "Humidifier") {
                auto humidifier = new SmartHumidifier();
                istringstream iss(deviceData);
                iss >> *humidifier;
                device = humidifier;
            } else if (deviceType == "Dehumidifier") {
                auto dehumidifier = new SmartDehumidifier();
                istringstream iss(deviceData);
                iss >> *dehumidifier;
                device = dehumidifier;
            } else if (deviceType == "Fan") {
                auto fan = new SmartFan();
                istringstream iss(deviceData);
                iss >> *fan;
                device = fan;
            } else if (deviceType == "Light") {
                auto light = new SmartLight();
                istringstream iss(deviceData);
                iss >> *light;
                device = light;
            }
        } catch (...) {
            delete device;
            delete record;
            continue;
        }
        
        if (device) {
            devices.pushBack(device);
        }
        delete record;
    }
}

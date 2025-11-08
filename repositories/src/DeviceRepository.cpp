#include "../include/DeviceRepository.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

DeviceRepository::DeviceRepository(const string& file) : filename(file) {}

void DeviceRepository::saveDevice(const SmartDevice& device) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        throw FileException(60, "Failed to open file " + filename + " for writing");
    }
    
    file << device.getId() << "|" << device.getDeviceName() << "|" 
         << device.getLocation() << "|" << static_cast<int>(device.getMode()) 
         << "|" << device.getPowerLevel() << "\n";
    file.close();
}

vector<shared_ptr<SmartDevice>> DeviceRepository::loadAllDevices() {
    vector<shared_ptr<SmartDevice>> devices;
    ifstream file(filename);
    
    if (!file.is_open()) {
        return devices;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        // Загрузка устройств - упрощенная версия
        // В реальной реализации нужно сохранять тип устройства
    }
    
    file.close();
    return devices;
}

shared_ptr<SmartDevice> DeviceRepository::findDeviceById(int deviceId) {
    auto devices = loadAllDevices();
    for (const auto& device : devices) {
        if (device->getId() == deviceId) {
            return device;
        }
    }
    throw FileException(61, "Device with ID " + to_string(deviceId) + " not found");
}

void DeviceRepository::updateDevice(const SmartDevice& updatedDevice) {
    auto devices = loadAllDevices();
    bool found = false;
    
    for (size_t i = 0; i < devices.size(); ++i) {
        if (devices[i]->getId() == updatedDevice.getId()) {
            *devices[i] = updatedDevice;
            found = true;
            break;
        }
    }
    
    if (!found) {
        throw FileException(62, "Device with ID " + to_string(updatedDevice.getId()) + " not found for update");
    }
    
    // Перезаписываем файл
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) {
        throw FileException(63, "Failed to open file " + filename + " for update");
    }
    
    for (const auto& device : devices) {
        file << device->getId() << "|" << device->getDeviceName() << "|" 
             << device->getLocation() << "|" << static_cast<int>(device->getMode()) 
             << "|" << device->getPowerLevel() << "\n";
    }
    
    file.close();
}

void DeviceRepository::deleteDevice(int deviceId) {
    auto devices = loadAllDevices();
    auto it = remove_if(devices.begin(), devices.end(),
        [deviceId](const shared_ptr<SmartDevice>& d) { return d->getId() == deviceId; });
    
    if (it == devices.end()) {
        throw FileException(64, "Device with ID " + to_string(deviceId) + " not found for deletion");
    }
    
    devices.erase(it, devices.end());
    
    // Перезаписываем файл
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) {
        throw FileException(65, "Failed to open file " + filename + " for deletion");
    }
    
    for (const auto& device : devices) {
        file << device->getId() << "|" << device->getDeviceName() << "|" 
             << device->getLocation() << "|" << static_cast<int>(device->getMode()) 
             << "|" << device->getPowerLevel() << "\n";
    }
    
    file.close();
}

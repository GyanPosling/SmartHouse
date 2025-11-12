#include "../include/DeviceContainer.hpp"
#include "../../exceptions/include/ContainerException.hpp"
#include "../../models/include/devices/Device.hpp"
#include "../../models/include/devices/SmartDevice.hpp"
#include <sstream>
#include <string>
using namespace std;

DeviceContainer::DeviceContainer() {}

void DeviceContainer::addDevice(shared_ptr<SmartDevice> device) {
    if (device == nullptr) {
        throw ContainerException(100, "Attempt to add empty device to container");
    }
    devices.push_back(device);
}

void DeviceContainer::removeDevice(int deviceId) {
    auto it = remove_if(devices.begin(), devices.end(),
        [deviceId](const shared_ptr<SmartDevice>& d) {
            return d->getId() == deviceId;
        });
    
    if (it == devices.end()) {
        throw ContainerException(101, "Device with ID " + to_string(deviceId) + " not found");
    }
    
    devices.erase(it, devices.end());
}

shared_ptr<SmartDevice> DeviceContainer::findDeviceById(int deviceId) const {
    for (const auto& device : devices) {
        if (device->getId() == deviceId) {
            return device;
        }
    }
    return nullptr;
}

vector<shared_ptr<SmartDevice>> DeviceContainer::searchByField(DeviceSearchField field, const string& value) const {
    vector<shared_ptr<SmartDevice>> results;
    
    // Устанавливаем режим поиска
    Device::setSearchMode(field);
    
    // Создаем временное устройство для сравнения
    // Для поиска по строковым полям создаем устройство с нужным значением
    Device searchDevice;
    switch (field) {
        case DeviceSearchField::ID:
            searchDevice.setId(stoi(value));
            break;
        case DeviceSearchField::DEVICE_NAME:
            searchDevice.setDeviceName(value);
            break;
        case DeviceSearchField::LOCATION:
            searchDevice.setLocation(value);
            break;
        case DeviceSearchField::POWER_LEVEL:
            searchDevice.setPowerLevel(stoi(value));
            break;
        case DeviceSearchField::MODE:
            // Для MODE будет обработано в SmartDevice
            break;
    }
    
    for (const auto& device : devices) {
        // Используем перегрузку == для поиска
        if (field == DeviceSearchField::MODE) {
            // Для MODE проверяем через SmartDevice
            SmartDevice* sd = dynamic_cast<SmartDevice*>(device.get());
            if (sd && sd->getModeString() == value) {
                results.push_back(device);
            }
        } else {
            // Для остальных полей используем базовый класс
            // Приводим SmartDevice к Device для сравнения
            Device* baseDevice = dynamic_cast<Device*>(device.get());
            if (baseDevice && *baseDevice == searchDevice) {
                results.push_back(device);
            }
        }
    }
    
    return results;
}

void DeviceContainer::sortByField(DeviceSortField field) {
    // Преобразуем DeviceSortField в DeviceSearchField для установки режима
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
    
    // Устанавливаем режим сортировки
    Device::setSearchMode(searchField);
    
    // Используем перегрузку < для сортировки
    sort(devices.begin(), devices.end(),
        [](const shared_ptr<SmartDevice>& a, const shared_ptr<SmartDevice>& b) {
            return *a < *b;
        });
}

size_t DeviceContainer::size() const {
    return devices.size();
}

bool DeviceContainer::empty() const {
    return devices.empty();
}

void DeviceContainer::clear() {
    devices.clear();
}

DeviceContainer::Iterator DeviceContainer::begin() {
    return Iterator(devices.begin());
}

DeviceContainer::Iterator DeviceContainer::end() {
    return Iterator(devices.end());
}

shared_ptr<SmartDevice> DeviceContainer::operator[](size_t index) const {
    if (index >= devices.size()) {
        throw ContainerException(102, "Index out of container bounds");
    }
    return devices[index];
}


DeviceContainer::Iterator::Iterator(vector<shared_ptr<SmartDevice>>::iterator iterator) : it(iterator) {}

DeviceContainer::Iterator& DeviceContainer::Iterator::operator++() {
    ++it;
    return *this;
}

DeviceContainer::Iterator DeviceContainer::Iterator::operator++(int) {
    Iterator temp = *this;
    ++it;
    return temp;
}

DeviceContainer::Iterator& DeviceContainer::Iterator::operator--() {
    --it;
    return *this;
}

DeviceContainer::Iterator DeviceContainer::Iterator::operator--(int) {
    Iterator temp = *this;
    --it;
    return temp;
}

shared_ptr<SmartDevice>& DeviceContainer::Iterator::operator*() {
    return *it;
}

shared_ptr<SmartDevice>* DeviceContainer::Iterator::operator->() {
    return it.operator->();
}

bool DeviceContainer::Iterator::operator==(const Iterator& other) const {
    return it == other.it;
}

bool DeviceContainer::Iterator::operator!=(const Iterator& other) const {
    return it != other.it;
}

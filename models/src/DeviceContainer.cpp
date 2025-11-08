#include "../include/DeviceContainer.hpp"
#include "../../exceptions/include/ContainerException.hpp"
#include <sstream>

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
    
    for (const auto& device : devices) {
        bool match = false;
        
        switch (field) {
            case DeviceSearchField::ID:
                if (to_string(device->getId()) == value) {
                    match = true;
                }
                break;
            case DeviceSearchField::DEVICE_NAME:
                if (device->getDeviceName() == value) {
                    match = true;
                }
                break;
            case DeviceSearchField::LOCATION:
                if (device->getLocation() == value) {
                    match = true;
                }
                break;
            case DeviceSearchField::MODE:
                if (device->getModeString() == value) {
                    match = true;
                }
                break;
            case DeviceSearchField::POWER_LEVEL:
                if (to_string(device->getPowerLevel()) == value) {
                    match = true;
                }
                break;
        }
        
        if (match) {
            results.push_back(device);
        }
    }
    
    return results;
}

void DeviceContainer::sortByField(DeviceSortField field) {
    sort(devices.begin(), devices.end(),
        [field](const shared_ptr<SmartDevice>& a, const shared_ptr<SmartDevice>& b) {
            switch (field) {
                case DeviceSortField::ID:
                    return a->getId() < b->getId();
                case DeviceSortField::DEVICE_NAME:
                    return a->getDeviceName() < b->getDeviceName();
                case DeviceSortField::LOCATION:
                    return a->getLocation() < b->getLocation();
                case DeviceSortField::MODE:
                    return static_cast<int>(a->getMode()) < static_cast<int>(b->getMode());
                case DeviceSortField::POWER_LEVEL:
                    return a->getPowerLevel() < b->getPowerLevel();
                default:
                    return false;
            }
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

// Iterator implementation
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

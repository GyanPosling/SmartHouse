#include "../include/SmartHome.hpp"
#include <algorithm>

SmartHome::SmartHome() : id(0), user() {}

SmartHome::SmartHome(int homeId, const User& homeUser) : id(homeId), user(homeUser) {}

int SmartHome::getId() const {
    return id;
}

void SmartHome::setId(int homeId) {
    id = homeId;
}

User SmartHome::getUser() const {
    return user;
}

void SmartHome::setUser(const User& homeUser) {
    user = homeUser;
}

void SmartHome::addDevice(shared_ptr<SmartDevice> device) {
    devices.push_back(device);
}

void SmartHome::removeDevice(int deviceId) {
    devices.erase(
        remove_if(devices.begin(), devices.end(),
            [deviceId](const shared_ptr<SmartDevice>& d) {
                return d->getId() == deviceId;
            }),
        devices.end()
    );
}

shared_ptr<SmartDevice> SmartHome::getDeviceById(int deviceId) const {
    for (const auto& device : devices) {
        if (device->getId() == deviceId) {
            return device;
        }
    }
    return nullptr;
}

vector<shared_ptr<SmartDevice>> SmartHome::getDevices() const {
    return devices;
}

void SmartHome::updateAllDevices(double temperature, double humidity, double co2, int hour) {
    for (auto& device : devices) {
        device->update(temperature, humidity, co2, hour);
    }
}

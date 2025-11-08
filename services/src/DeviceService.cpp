#include "../include/DeviceService.hpp"
#include "../exceptions/include/ContainerException.hpp"

DeviceService::DeviceService() 
    : deviceRepository(make_unique<DeviceRepository>("devices.txt")),
      sensorRepository(make_unique<SensorRepository>("sensors.txt")),
      environmentSimulator(make_shared<EnvironmentSimulator>()) {
    automationService = make_unique<AutomationService>(environmentSimulator);
    loadAllDevices();
}

void DeviceService::addDevice(shared_ptr<SmartDevice> device) {
    deviceContainer.addDevice(device);
    deviceRepository->saveDevice(*device);
}

void DeviceService::removeDevice(int deviceId) {
    deviceContainer.removeDevice(deviceId);
    deviceRepository->deleteDevice(deviceId);
}

void DeviceService::updateDevice(int deviceId, shared_ptr<SmartDevice> updatedDevice) {
    auto device = deviceContainer.findDeviceById(deviceId);
    if (device == nullptr) {
        throw ContainerException(103, "Device with ID " + to_string(deviceId) + " not found");
    }
    
    device = updatedDevice;
    deviceRepository->updateDevice(*device);
}

shared_ptr<SmartDevice> DeviceService::getDeviceById(int deviceId) {
    return deviceContainer.findDeviceById(deviceId);
}

vector<shared_ptr<SmartDevice>> DeviceService::getAllDevices() {
    vector<shared_ptr<SmartDevice>> devices;
    for (auto it = deviceContainer.begin(); it != deviceContainer.end(); ++it) {
        devices.push_back(*it);
    }
    return devices;
}

vector<shared_ptr<SmartDevice>> DeviceService::searchDevices(DeviceSearchField field, const string& value) {
    return deviceContainer.searchByField(field, value);
}

void DeviceService::sortDevices(DeviceSortField field) {
    deviceContainer.sortByField(field);
}

void DeviceService::updateAllDevices() {
    vector<shared_ptr<SmartDevice>> devices = getAllDevices();
    automationService->updateDevices(devices);
}

void DeviceService::saveAllDevices() {
    // Устройства сохраняются автоматически при добавлении/обновлении
}

void DeviceService::loadAllDevices() {
    try {
        auto devices = deviceRepository->loadAllDevices();
        // Загрузка устройств - нужно будет адаптировать под новую иерархию
    } catch (...) {
        // Игнорируем ошибки при первом запуске
    }
}

double DeviceService::getCurrentTemperature() const {
    return automationService->getCurrentTemperature();
}

double DeviceService::getCurrentHumidity() const {
    return automationService->getCurrentHumidity();
}

double DeviceService::getCurrentCO2() const {
    return automationService->getCurrentCO2();
}

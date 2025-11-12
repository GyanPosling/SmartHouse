#include "../include/DeviceService.hpp"
#include "../exceptions/include/ContainerException.hpp"
#include <ctime>

DeviceService::DeviceService() 
    : deviceRepository(make_unique<DeviceRepository>("devices.txt")),
      sensorRepository(make_unique<SensorRepository>("sensors.txt")),
      environmentSimulator(make_shared<EnvironmentSimulator>()) {
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
    // Обновляем показатели среды
    environmentSimulator->updateEnvironment();
    
    double temperature = environmentSimulator->getTemperature();
    double humidity = environmentSimulator->getHumidity();
    double co2 = environmentSimulator->getCO2();
    
    // Получаем текущий час
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    int hour = localTime->tm_hour;
    
    // Обновляем все устройства
    vector<shared_ptr<SmartDevice>> devices = getAllDevices();
    for (auto& device : devices) {
        if (device->getMode() == DeviceMode::AUTOMATIC) {
            // Только устройства в автоматическом режиме обновляются
            device->update(temperature, humidity, co2, hour);
        }
        // Устройства в ручном режиме остаются в текущем состоянии
    }
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
    return environmentSimulator->getTemperature();
}

double DeviceService::getCurrentHumidity() const {
    return environmentSimulator->getHumidity();
}

double DeviceService::getCurrentCO2() const {
    return environmentSimulator->getCO2();
}

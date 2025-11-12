#pragma once
#include "../../models/include/DeviceContainer.hpp"
#include "../../models/include/devices/SmartDevice.hpp"
#include "../../models/include/EnvironmentSimulator.hpp"
#include "../../repositories/include/DeviceRepository.hpp"
#include "../../repositories/include/SensorRepository.hpp"
#include <memory>
#include <vector>
#include <ctime>
using namespace std;

class DeviceService {
private:
    DeviceContainer deviceContainer;
    unique_ptr<DeviceRepository> deviceRepository;
    unique_ptr<SensorRepository> sensorRepository;
    shared_ptr<EnvironmentSimulator> environmentSimulator;

public:
    DeviceService();
    
    void addDevice(shared_ptr<SmartDevice> device);
    void removeDevice(int deviceId);
    void updateDevice(int deviceId, shared_ptr<SmartDevice> updatedDevice);
    shared_ptr<SmartDevice> getDeviceById(int deviceId);
    vector<shared_ptr<SmartDevice>> getAllDevices();
    
    vector<shared_ptr<SmartDevice>> searchDevices(DeviceSearchField field, const string& value);
    void sortDevices(DeviceSortField field);
    
    void updateAllDevices();
    void saveAllDevices();
    void loadAllDevices();
    
    double getCurrentTemperature() const;
    double getCurrentHumidity() const;
    double getCurrentCO2() const;
};

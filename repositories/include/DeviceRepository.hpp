#pragma once
#include "../../models/include/devices/SmartDevice.hpp"
#include "../../exceptions/include/FileException.hpp"
#include <vector>
#include <memory>
#include <string>
using namespace std;

class DeviceRepository {
private:
    string filename;

public:
    DeviceRepository(const string& file);
    
    void saveDevice(const SmartDevice& device);
    vector<shared_ptr<SmartDevice>> loadAllDevices();
    shared_ptr<SmartDevice> findDeviceById(int deviceId);
    void updateDevice(const SmartDevice& device);
    void deleteDevice(int deviceId);
};

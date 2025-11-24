#pragma once
#include "../../models/include/devices/SmartDevice.hpp"
#include "../../models/include/devices/SmartAirConditioner.hpp"
#include "../../models/include/devices/SmartHeater.hpp"
#include "../../models/include/devices/SmartHumidifier.hpp"
#include "../../models/include/devices/SmartDehumidifier.hpp"
#include "../../models/include/devices/SmartFan.hpp"
#include "../../models/include/devices/SmartLight.hpp"
#include "../../repositories/include/TextFile.hpp"
#include "../../exceptions/include/FileException.hpp"
#include <memory>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class DeviceService {
private:
    vector<shared_ptr<SmartDevice>> devices;
    TextFile<string> deviceFile;

public:
    DeviceService();
    
    void addDevice(shared_ptr<SmartDevice> device);
    void removeDevice(int deviceId);
    void updateDevice(int deviceId, shared_ptr<SmartDevice> updatedDevice);
    shared_ptr<SmartDevice> getDeviceById(int deviceId);
    const vector<shared_ptr<SmartDevice>> getAllDevices() const;
    
    vector<shared_ptr<SmartDevice>> searchDevices(int option);
    void sortDevices(int option);
    
    void saveAllDevices();
    void loadAllDevices();
};
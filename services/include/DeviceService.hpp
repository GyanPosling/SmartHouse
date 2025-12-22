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
#include "../../templates/Deque.hpp"
#include "../../templates/Algorithm.hpp"
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class DeviceService {
private:
    Deque<SmartDevice*> devices;
    TextFile<string> deviceFile;
    Algorithm<SmartDevice*> algorithm;

    void deleteAll();

public:
    DeviceService();
    ~DeviceService();
    
    void addDevice(SmartDevice* device);
    void removeDevice(int deviceId);
    void updateDevice(int deviceId, SmartDevice* updatedDevice);
    SmartDevice* getDeviceById(int deviceId);
    Deque<SmartDevice*> getAllDevices() const;
    
    Deque<SmartDevice*> searchDevices(int option);
    Deque<SmartDevice*> searchDevices(DeviceSearchField field, const SmartDevice& sample);
    void sortDevices(int option);
    
    void saveAllDevices();
    void loadAllDevices();
}; 

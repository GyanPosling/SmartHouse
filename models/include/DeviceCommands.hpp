#pragma once
#include "Command.hpp"
#include "../services/include/DeviceService.hpp"
#include "devices/SmartDevice.hpp"
#include <memory>
using namespace std;

class AddDeviceCommand : public Command {
private:
    DeviceService* deviceService;
    shared_ptr<SmartDevice> device;

public:
    AddDeviceCommand(DeviceService* service, shared_ptr<SmartDevice> dev)
        : deviceService(service), device(dev) {}
    
    void execute() override {
        deviceService->addDevice(device);
    }
    
    void undo() override {
        deviceService->removeDevice(device->getId());
    }
};

class RemoveDeviceCommand : public Command {
private:
    DeviceService* deviceService;
    shared_ptr<SmartDevice> device;
    int deviceId;

public:
    RemoveDeviceCommand(DeviceService* service, int id)
        : deviceService(service), deviceId(id) {
        device = deviceService->getDeviceById(id);
    }
    
    void execute() override {
        deviceService->removeDevice(deviceId);
    }
    
    void undo() override {
        deviceService->addDevice(device);
    }
};


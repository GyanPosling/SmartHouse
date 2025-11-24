#include "../include/DeviceCommands.hpp"
using namespace std;

AddDeviceCommand::AddDeviceCommand(DeviceService* service, shared_ptr<SmartDevice> dev)
    : deviceService(service), device(dev) {}

void AddDeviceCommand::execute() {
    deviceService->addDevice(device);
}

void AddDeviceCommand::undo() {
    deviceService->removeDevice(device->getId());
}
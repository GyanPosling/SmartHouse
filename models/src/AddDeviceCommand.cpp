#include "../include/DeviceCommands.hpp"
using namespace std;

namespace {
SmartDevice* cloneDevice(const SmartDevice* device) {
    if (auto ac = dynamic_cast<const SmartAirConditioner*>(device)) {
        return new SmartAirConditioner(*ac);
    } else if (auto heater = dynamic_cast<const SmartHeater*>(device)) {
        return new SmartHeater(*heater);
    } else if (auto humidifier = dynamic_cast<const SmartHumidifier*>(device)) {
        return new SmartHumidifier(*humidifier);
    } else if (auto dehumidifier = dynamic_cast<const SmartDehumidifier*>(device)) {
        return new SmartDehumidifier(*dehumidifier);
    } else if (auto fan = dynamic_cast<const SmartFan*>(device)) {
        return new SmartFan(*fan);
    } else if (auto light = dynamic_cast<const SmartLight*>(device)) {
        return new SmartLight(*light);
    }
    return new SmartDevice(*device);
}
}

AddDeviceCommand::AddDeviceCommand(DeviceService* service, SmartDevice* dev)
    : deviceService(service), device(dev ? cloneDevice(dev) : nullptr) {
    delete dev;
}

AddDeviceCommand::~AddDeviceCommand() {
    delete device;
}

void AddDeviceCommand::execute() {
    if (!deviceService || !device) return;
    SmartDevice* copy = cloneDevice(device);
    deviceService->addDevice(copy);
}

void AddDeviceCommand::undo() {
    if (!deviceService || !device) return;
    deviceService->removeDevice(device->getId());
}

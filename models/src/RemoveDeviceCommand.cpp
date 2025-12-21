#include"../include/RemoveDeviceCommand.hpp"

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

RemoveDeviceCommand::RemoveDeviceCommand(DeviceService* service, int deviceId)
    : deviceService(service), removedDevice(nullptr) {

    SmartDevice* existing = deviceService->getDeviceById(deviceId);
    if (!existing) {
        throw runtime_error("Cannot create Remove command: device with ID " + to_string(deviceId) + " not found.");
    }
    removedDevice = cloneDevice(existing);
}

RemoveDeviceCommand::~RemoveDeviceCommand() {
    delete removedDevice;
}

void RemoveDeviceCommand::execute() {
    if (!deviceService || !removedDevice) return;
    deviceService->removeDevice(removedDevice->getId());
}

void RemoveDeviceCommand::undo() {
    if (!deviceService || !removedDevice) return;
    deviceService->addDevice(cloneDevice(removedDevice));
}

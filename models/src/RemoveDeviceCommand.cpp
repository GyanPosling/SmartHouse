#include"../include/RemoveDeviceCommand.hpp"

RemoveDeviceCommand::RemoveDeviceCommand(DeviceService* service, int deviceId)
    : deviceService(service) {

    removedDevice = deviceService->getDeviceById(deviceId);
    if (!removedDevice) {
        throw runtime_error("Cannot create Remove command: device with ID " + to_string(deviceId) + " not found.");
    }
}

void RemoveDeviceCommand::execute() {
    deviceService->removeDevice(removedDevice->getId());
}

void RemoveDeviceCommand::undo() {
    deviceService->addDevice(removedDevice);
}
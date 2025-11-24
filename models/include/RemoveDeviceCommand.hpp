#pragma once
#include "Command.hpp"
#include "../../services/include/DeviceService.hpp"
#include "devices/SmartDevice.hpp"
#include <memory>

class RemoveDeviceCommand : public Command {
private:
    DeviceService* deviceService;
    shared_ptr<SmartDevice> removedDevice;  // Сохраняем само устройство для undo

public:
    RemoveDeviceCommand(DeviceService* service, int deviceId);
    void execute() override;
    void undo() override;
};
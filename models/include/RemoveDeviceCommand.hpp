#pragma once
#include "Command.hpp"
#include "../../services/include/DeviceService.hpp"
#include "devices/SmartDevice.hpp"

class RemoveDeviceCommand : public Command {
private:
    DeviceService* deviceService;
    SmartDevice* removedDevice;  // Сохраняем само устройство для undo

public:
    RemoveDeviceCommand(DeviceService* service, int deviceId);
    ~RemoveDeviceCommand();
    void execute() override;
    void undo() override;
};

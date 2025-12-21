#pragma once
#include "Command.hpp"
#include "../../services/include/DeviceService.hpp"
#include "devices/SmartDevice.hpp"
using namespace std;

class AddDeviceCommand : public Command {
private:
    DeviceService* deviceService;
    SmartDevice* device;

public:
    AddDeviceCommand(DeviceService* service, SmartDevice* dev);
    ~AddDeviceCommand();
    
    void execute() override;
    
    void undo() override;
};

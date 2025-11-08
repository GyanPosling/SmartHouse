#include <iostream>
#include <string>
#include <memory>
#include <locale>
using namespace std;

#include "../services/include/AuthenticationService.hpp"
#include "../services/include/DeviceService.hpp"
#include "../services/include/CommandHistory.hpp"
#include "../models/include/devices/SmartDevice.hpp"
#include "../models/include/devices/SmartAirConditioner.hpp"
#include "../models/include/devices/SmartHeater.hpp"
#include "../models/include/devices/SmartHumidifier.hpp"
#include "../models/include/devices/SmartDehumidifier.hpp"
#include "../models/include/devices/SmartFan.hpp"
#include "../models/include/devices/SmartLight.hpp"
#include "../utils/include/InputUtils.hpp"
#include "../exceptions/include/Exception.hpp"

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

void printMainMenu() {
    cout << "\n========== SMART HOME ==========\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Choose an action: ";
}

void printUserMenu() {
    cout << "\n========== MAIN MENU ==========\n";
    cout << "1. View all devices\n";
    cout << "2. Add device\n";
    cout << "3. Edit device\n";
    cout << "4. Delete device\n";
    cout << "5. Search devices\n";
    cout << "6. Sort devices\n";
    cout << "7. View device information\n";
    cout << "8. Switch device mode\n";
    cout << "9. Update device states\n";
    cout << "10. Undo last action\n";
    cout << "11. Redo last action\n";
    cout << "12. View environment indicators\n";
    cout << "0. Exit\n";
    cout << "Choose an action: ";
}

void printDeviceTypes() {
    cout << "\nDevice types:\n";
    cout << "1. Air Conditioner\n";
    cout << "2. Heater\n";
    cout << "3. Humidifier\n";
    cout << "4. Dehumidifier\n";
    cout << "5. Fan\n";
    cout << "6. Smart Light\n";
    cout << "Choose type: ";
}

void printModes() {
    cout << "\nOperation modes:\n";
    cout << "0. Automatic\n";
    cout << "1. Manual\n";
    cout << "2. Off\n";
    cout << "Choose mode: ";
}

void printSearchFields() {
    cout << "\nSearch fields:\n";
    cout << "1. ID\n";
    cout << "2. Device name\n";
    cout << "3. Location\n";
    cout << "4. Mode\n";
    cout << "5. Power level\n";
    cout << "Choose field: ";
}

void printSortFields() {
    cout << "\nSort fields:\n";
    cout << "1. ID\n";
    cout << "2. Device name\n";
    cout << "3. Location\n";
    cout << "4. Mode\n";
    cout << "5. Power level\n";
    cout << "Choose field: ";
}

DeviceSearchField getSearchField(int choice) {
    switch (choice) {
        case 1: return DeviceSearchField::ID;
        case 2: return DeviceSearchField::DEVICE_NAME;
        case 3: return DeviceSearchField::LOCATION;
        case 4: return DeviceSearchField::MODE;
        case 5: return DeviceSearchField::POWER_LEVEL;
        default: return DeviceSearchField::ID;
    }
}

DeviceSortField getSortField(int choice) {
    switch (choice) {
        case 1: return DeviceSortField::ID;
        case 2: return DeviceSortField::DEVICE_NAME;
        case 3: return DeviceSortField::LOCATION;
        case 4: return DeviceSortField::MODE;
        case 5: return DeviceSortField::POWER_LEVEL;
        default: return DeviceSortField::ID;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    AuthenticationService authService;
    DeviceService deviceService;
    CommandHistory commandHistory;
    bool running = true;
    bool loggedIn = false;
    
    while (running) {
        try {
            if (!loggedIn) {
                printMainMenu();
                int choice = InputUtils::readInt(cin, "");
                
                switch (choice) {
                    case 1: {
                        cout << "\n=== REGISTRATION ===\n";
                        string username = InputUtils::readString(cin, "Enter username: ");
                        string password = InputUtils::readString(cin, "Enter password: ");
                        
                        try {
                            User newUser = authService.registerUser(username, password);
                            cout << "Registration successful! User ID: " << newUser.getId() << "\n";
                        } catch (const Exception& e) {
                            cout << "Error: " << e.what() << "\n";
                        }
                        break;
                    }
                    case 2: {
                        cout << "\n=== LOGIN ===\n";
                        string username = InputUtils::readString(cin, "Enter username: ");
                        string password = InputUtils::readString(cin, "Enter password: ");
                        
                        try {
                            authService.loginUser(username, password);
                            loggedIn = true;
                            cout << "Login successful!\n";
                        } catch (const Exception& e) {
                            cout << "Error: " << e.what() << "\n";
                        }
                        break;
                    }
                    case 3:
                        running = false;
                        break;
                    default:
                        cout << "Invalid choice!\n";
                }
            } else {
                printUserMenu();
                int choice = InputUtils::readInt(cin, "");
                
                switch (choice) {
                    case 1: {
                        cout << "\n=== ALL DEVICES ===\n";
                        auto devices = deviceService.getAllDevices();
                        if (devices.empty()) {
                            cout << "No devices found.\n";
                        } else {
                            for (const auto& device : devices) {
                                cout << device->getDeviceInfo() << "\n";
                            }
                        }
                        break;
                    }
                    case 2: {
                        cout << "\n=== ADD DEVICE ===\n";
                        printDeviceTypes();
                        int deviceType = InputUtils::readInt(cin, "");
                        
                        int id = InputUtils::readInt(cin, "Enter device ID: ");
                        string name = InputUtils::readString(cin, "Enter device name: ");
                        string location = InputUtils::readString(cin, "Enter location: ");
                        printModes();
                        int modeInt = InputUtils::readInt(cin, "");
                        DeviceMode mode = static_cast<DeviceMode>(modeInt);
                        int power = InputUtils::readInt(cin, "Enter power level: ");
                        
                        shared_ptr<SmartDevice> device;
                        
                        if (deviceType == 1) {
                            double targetCO2 = InputUtils::readDouble(cin, "Enter target CO2: ");
                            double tolerance = InputUtils::readDouble(cin, "Enter tolerance: ");
                            device = make_shared<SmartAirConditioner>(
                                id, name, location, mode, power, targetCO2, tolerance);
                        } else if (deviceType == 2) {
                            double targetTemp = InputUtils::readDouble(cin, "Enter target temperature: ");
                            double tolerance = InputUtils::readDouble(cin, "Enter tolerance: ");
                            device = make_shared<SmartHeater>(
                                id, name, location, mode, power, targetTemp, tolerance);
                        } else if (deviceType == 3) {
                            double targetHumidity = InputUtils::readDouble(cin, "Enter target humidity: ");
                            double tolerance = InputUtils::readDouble(cin, "Enter tolerance: ");
                            device = make_shared<SmartHumidifier>(
                                id, name, location, mode, power, targetHumidity, tolerance);
                        } else if (deviceType == 4) {
                            double targetHumidity = InputUtils::readDouble(cin, "Enter target humidity: ");
                            double tolerance = InputUtils::readDouble(cin, "Enter tolerance: ");
                            device = make_shared<SmartDehumidifier>(
                                id, name, location, mode, power, targetHumidity, tolerance);
                        } else if (deviceType == 5) {
                            double targetCO2 = InputUtils::readDouble(cin, "Enter target CO2: ");
                            double tolerance = InputUtils::readDouble(cin, "Enter tolerance: ");
                            device = make_shared<SmartFan>(
                                id, name, location, mode, power, targetCO2, tolerance);
                        } else if (deviceType == 6) {
                            int offHour = InputUtils::readInt(cin, "Enter turn off hour (0-23): ");
                            device = make_shared<SmartLight>(
                                id, name, location, mode, power, offHour);
                        } else {
                            cout << "Invalid device type!\n";
                            break;
                        }
                        
                        try {
                            commandHistory.executeCommand(
                                make_unique<AddDeviceCommand>(&deviceService, device));
                            cout << "Device added successfully!\n";
                        } catch (const Exception& e) {
                            cout << "Error: " << e.what() << "\n";
                        }
                        break;
                    }
                    case 3: {
                        cout << "\n=== EDIT DEVICE ===\n";
                        int id = InputUtils::readInt(cin, "Enter device ID: ");
                        
                        try {
                            auto device = deviceService.getDeviceById(id);
                            if (device == nullptr) {
                                cout << "Device not found!\n";
                                break;
                            }
                            
                            string name = InputUtils::readString(cin, "Enter new device name: ");
                            string location = InputUtils::readString(cin, "Enter new location: ");
                            printModes();
                            int modeInt = InputUtils::readInt(cin, "");
                            DeviceMode mode = static_cast<DeviceMode>(modeInt);
                            int power = InputUtils::readInt(cin, "Enter new power level: ");
                            
                            device->setDeviceName(name);
                            device->setLocation(location);
                            device->setMode(mode);
                            device->setPowerLevel(power);
                            
                            deviceService.updateDevice(id, device);
                            cout << "Device updated successfully!\n";
                        } catch (const Exception& e) {
                            cout << "Error: " << e.what() << "\n";
                        }
                        break;
                    }
                    case 4: {
                        cout << "\n=== DELETE DEVICE ===\n";
                        int id = InputUtils::readInt(cin, "Enter device ID: ");
                        
                        try {
                            commandHistory.executeCommand(
                                make_unique<RemoveDeviceCommand>(&deviceService, id));
                            cout << "Device deleted successfully!\n";
                        } catch (const Exception& e) {
                            cout << "Error: " << e.what() << "\n";
                        }
                        break;
                    }
                    case 5: {
                        cout << "\n=== SEARCH DEVICES ===\n";
                        printSearchFields();
                        int fieldChoice = InputUtils::readInt(cin, "");
                        DeviceSearchField field = getSearchField(fieldChoice);
                        string value = InputUtils::readString(cin, "Enter search value: ");
                        
                        try {
                            auto results = deviceService.searchDevices(field, value);
                            if (results.empty()) {
                                cout << "No devices found.\n";
                            } else {
                                cout << "Found devices: " << results.size() << "\n";
                                for (const auto& device : results) {
                                    cout << device->getDeviceInfo() << "\n";
                                }
                            }
                        } catch (const Exception& e) {
                            cout << "Error: " << e.what() << "\n";
                        }
                        break;
                    }
                    case 6: {
                        cout << "\n=== SORT DEVICES ===\n";
                        printSortFields();
                        int fieldChoice = InputUtils::readInt(cin, "");
                        DeviceSortField field = getSortField(fieldChoice);
                        
                        try {
                            deviceService.sortDevices(field);
                            cout << "Devices sorted.\n";
                            auto devices = deviceService.getAllDevices();
                            for (const auto& device : devices) {
                                cout << device->getDeviceInfo() << "\n";
                            }
                        } catch (const Exception& e) {
                            cout << "Error: " << e.what() << "\n";
                        }
                        break;
                    }
                    case 7: {
                        cout << "\n=== DEVICE INFORMATION ===\n";
                        int id = InputUtils::readInt(cin, "Enter device ID: ");
                        
                        try {
                            auto device = deviceService.getDeviceById(id);
                            if (device == nullptr) {
                                cout << "Device not found!\n";
                            } else {
                                cout << device->getDeviceInfo() << "\n";
                            }
                        } catch (const Exception& e) {
                            cout << "Error: " << e.what() << "\n";
                        }
                        break;
                    }
                    case 8: {
                        cout << "\n=== SWITCH DEVICE MODE ===\n";
                        int id = InputUtils::readInt(cin, "Enter device ID: ");
                        
                        try {
                            auto device = deviceService.getDeviceById(id);
                            if (device == nullptr) {
                                cout << "Device not found!\n";
                                break;
                            }
                            
                            printModes();
                            int modeInt = InputUtils::readInt(cin, "");
                            device->setMode(static_cast<DeviceMode>(modeInt));
                            deviceService.updateDevice(id, device);
                            cout << "Mode changed successfully!\n";
                        } catch (const Exception& e) {
                            cout << "Error: " << e.what() << "\n";
                        }
                        break;
                    }
                    case 9: {
                        cout << "\n=== UPDATE DEVICE STATES ===\n";
                        deviceService.updateAllDevices();
                        cout << "Device states updated.\n";
                        break;
                    }
                    case 10: {
                        if (commandHistory.canUndo()) {
                            commandHistory.undo();
                            cout << "Action undone.\n";
                        } else {
                            cout << "No actions to undo.\n";
                        }
                        break;
                    }
                    case 11: {
                        if (commandHistory.canRedo()) {
                            commandHistory.redo();
                            cout << "Action redone.\n";
                        } else {
                            cout << "No actions to redo.\n";
                        }
                        break;
                    }
                    case 12: {
                        cout << "\n=== ENVIRONMENT INDICATORS ===\n";
                        cout << "Temperature: " << deviceService.getCurrentTemperature() << "°C\n";
                        cout << "Humidity: " << deviceService.getCurrentHumidity() << "%\n";
                        cout << "CO2: " << deviceService.getCurrentCO2() << " ppm\n";
                        break;
                    }
                    case 0:
                        loggedIn = false;
                        authService.logout();
                        break;
                    default:
                        cout << "Invalid choice!\n";
                }
            }
        } catch (const Exception& e) {
            cout << "Error: " << e.what() << "\n";
        } catch (const exception& e) {
            cout << "Unexpected error: " << e.what() << "\n";
        }
    }
    
    return 0;
}

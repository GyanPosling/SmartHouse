#include "../include/Menu.hpp"
#include "../../services/include/ClimateFileManager.hpp"
#include "../../services/include/ClimateNormalizer.hpp"
#include "../../models/include/devices/SmartDevice.hpp"
#include "../../models/include/devices/SmartAirConditioner.hpp"
#include "../../models/include/devices/SmartHeater.hpp"
#include "../../models/include/devices/SmartHumidifier.hpp"
#include "../../models/include/devices/SmartDehumidifier.hpp"
#include "../../models/include/devices/SmartFan.hpp"
#include "../../models/include/devices/SmartLight.hpp"
#include "../../utils/include/InputUtils.hpp"
#include "../../exceptions/include/Exception.hpp"
#include "../../services/include/CommandHistory.hpp"
#include <iostream>
#include <iomanip>
#include <memory>
using namespace std;

// Классы команд для undo/redo
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

// Вспомогательная функция для рисования меню
void drawMenu(const string& title, const string options[], int numOptions) {
    int max_len = title.length();
    for (int i = 0; i < numOptions; i++) {
        if (options[i].length() > max_len) max_len = options[i].length();
    }
    max_len += 4;
    
    cout << "+" << string(max_len, '-') << "+" << endl;
    cout << "| " << title << string(max_len - title.length() - 1, ' ') << "|" << endl;
    cout << "+" << string(max_len, '-') << "+" << endl;
    for (int i = 0; i < numOptions; i++) {
        cout << "| " << options[i] << string(max_len - options[i].length() - 1, ' ') << "|" << endl;
    }
    cout << "+" << string(max_len, '-') << "+" << endl;
}

Menu::Menu() : loggedIn(false) {
    setlocale(LC_ALL, "Russian");
    try {
        currentClimateData = getClimateData();
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

ClimateData Menu::getClimateData() {
    cout << "\n=== CLIMATE DATA SETUP ===\n";
    bool hasFile = InputUtils::readBool(cin, "Do you have a file with climate indicators?");
    
    ClimateData data;
    
    if (hasFile) {
        string filename = InputUtils::readString(cin, "Enter filename: ");
        try {
            data = ClimateFileManager::readFromFile(filename);
            cout << "\nData loaded from file: " << filename << "\n";
            cout << fixed << setprecision(2);
            cout << "Temperature: " << data.getTemperature() << "°C\n";
            cout << "Humidity: " << data.getHumidity() << "%\n";
            cout << "CO2: " << data.getCO2() << " ppm\n";
        } catch (const Exception& e) {
            cout << "Error reading file: " << e.what() << "\n";
            cout << "Please enter data manually:\n";
            data.setTemperature(InputUtils::readDouble(cin, "Enter temperature: "));
            data.setHumidity(InputUtils::readDouble(cin, "Enter humidity: "));
            data.setCO2(InputUtils::readDouble(cin, "Enter CO2 level: "));
        }
    } else {
        cout << "Please enter climate indicators:\n";
        data.setTemperature(InputUtils::readDouble(cin, "Enter temperature: "));
        data.setHumidity(InputUtils::readDouble(cin, "Enter humidity: "));
        data.setCO2(InputUtils::readDouble(cin, "Enter CO2 level: "));
    }
    
    return data;
}

void Menu::printMainMenu() {
    string mainMenuOptions[] = {
        "1. Register",
        "2. Login",
        "3. Exit"
    };
    drawMenu("SMART HOME", mainMenuOptions, 3);
    cout << "Choose an action: ";
}

void Menu::printUserMenu() {
    string userMenuOptions[] = {
        "1. View all devices",
        "2. Add device",
        "3. Edit device",
        "4. Delete device",
        "5. Search devices",
        "6. Sort devices",
        "7. View device information",
        "8. Switch device mode",
        "9. Update device states",
        "10. Undo last action",
        "11. Redo last action",
        "12. View environment indicators",
        "13. Normalize smart home climate",
        "0. Exit"
    };
    drawMenu("MAIN MENU", userMenuOptions, 14);
    cout << "Choose an action: ";
}

void Menu::registerUser() {
    try {
        cout << "\n=== REGISTRATION ===\n";
        string username = InputUtils::readString(cin, "Enter username: ");
        string password = InputUtils::readString(cin, "Enter password: ");
        
        string birthday = InputUtils::readDate(cin, "Enter birthday");
        User newUser = authService.registerUser(username, password);
        newUser.setBirthday(birthday);
        cout << "Registration successful! User ID: " << newUser.getId() << "\n";
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::login() {
    try {
        cout << "\n=== LOGIN ===\n";
        string username = InputUtils::readString(cin, "Enter username: ");
        string password = InputUtils::readString(cin, "Enter password: ");
        
        authService.loginUser(username, password);
        loggedIn = true;
        cout << "Login successful!\n";
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::viewAllDevices() {
    cout << "\n=== ALL DEVICES ===\n";
    auto devices = deviceService.getAllDevices();
    if (devices.empty()) {
        cout << "No devices found.\n";
    } else {
        for (const auto& device : devices) {
            cout << device->getDeviceInfo() << "\n";
        }
    }
}

void Menu::addDevice() {
    cout << "\n=== ADD DEVICE ===\n";
    string deviceTypeOptions[] = {
        "1. Air Conditioner",
        "2. Heater",
        "3. Humidifier",
        "4. Dehumidifier",
        "5. Fan",
        "6. Smart Light"
    };
    drawMenu("Device Types", deviceTypeOptions, 6);
    cout << "Choose type: ";
    int deviceType = InputUtils::readIntInRange(cin, "", 1, 6);
    
    int id = InputUtils::readInt(cin, "Enter device ID: ");
    string name = InputUtils::readString(cin, "Enter device name: ");
    string location = InputUtils::readString(cin, "Enter location: ");
    
    string modeOptions[] = {
        "0. Automatic",
        "1. Manual",
        "2. Off"
    };
    drawMenu("Operation Modes", modeOptions, 3);
    cout << "Choose mode: ";
    int modeInt = InputUtils::readIntInRange(cin, "", 0, 2);
    DeviceMode mode = static_cast<DeviceMode>(modeInt);
    int power = InputUtils::readIntInRange(cin, "Enter power level (0-100): ", 0, 100);
    
    shared_ptr<SmartDevice> device;
    
    const double DEFAULT_TARGET_TEMP = 25.0;
    const double DEFAULT_TARGET_HUMIDITY = 30.0;
    const double DEFAULT_TARGET_CO2 = 400.0;
    const double DEFAULT_TOLERANCE = 5.0;
    
    if (deviceType == 1) {
        device = make_shared<SmartAirConditioner>(
            id, name, location, mode, power, DEFAULT_TARGET_TEMP, DEFAULT_TOLERANCE);
    } else if (deviceType == 2) {
        device = make_shared<SmartHeater>(
            id, name, location, mode, power, DEFAULT_TARGET_TEMP, DEFAULT_TOLERANCE);
    } else if (deviceType == 3) {
        device = make_shared<SmartHumidifier>(
            id, name, location, mode, power, DEFAULT_TARGET_HUMIDITY, DEFAULT_TOLERANCE);
    } else if (deviceType == 4) {
        device = make_shared<SmartDehumidifier>(
            id, name, location, mode, power, DEFAULT_TARGET_HUMIDITY, DEFAULT_TOLERANCE);
    } else if (deviceType == 5) {
        device = make_shared<SmartFan>(
            id, name, location, mode, power, DEFAULT_TARGET_CO2, DEFAULT_TOLERANCE);
    } else if (deviceType == 6) {
        int offHour = InputUtils::readIntInRange(cin, "Enter turn off hour (0-23): ", 0, 23);
        device = make_shared<SmartLight>(
            id, name, location, mode, power, offHour);
    }
    
    try {
        commandHistory.executeCommand(
            make_unique<AddDeviceCommand>(&deviceService, device));
        cout << "Device added successfully!\n";
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::editDevice() {
    cout << "\n=== EDIT DEVICE ===\n";
    int id = InputUtils::readInt(cin, "Enter device ID: ");
    
    try {
        auto device = deviceService.getDeviceById(id);
        if (device == nullptr) {
            cout << "Device not found!\n";
            return;
        }
        
        string name = InputUtils::readString(cin, "Enter new device name: ");
        string location = InputUtils::readString(cin, "Enter new location: ");
        
        string modeOptions[] = {
            "0. Automatic",
            "1. Manual",
            "2. Off"
        };
        drawMenu("Operation Modes", modeOptions, 3);
        cout << "Choose mode: ";
        int modeInt = InputUtils::readIntInRange(cin, "", 0, 2);
        DeviceMode mode = static_cast<DeviceMode>(modeInt);
        int power = InputUtils::readIntInRange(cin, "Enter new power level (0-100): ", 0, 100);
        
        device->setDeviceName(name);
        device->setLocation(location);
        device->setMode(mode);
        device->setPowerLevel(power);
        
        deviceService.updateDevice(id, device);
        cout << "Device updated successfully!\n";
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::deleteDevice() {
    cout << "\n=== DELETE DEVICE ===\n";
    int id = InputUtils::readInt(cin, "Enter device ID: ");
    
    try {
        commandHistory.executeCommand(
            make_unique<RemoveDeviceCommand>(&deviceService, id));
        cout << "Device deleted successfully!\n";
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::searchDevices() {
    cout << "\n=== SEARCH DEVICES ===\n";
    string searchOptions[] = {
        "1. ID",
        "2. Device name",
        "3. Location",
        "4. Mode",
        "5. Power level"
    };
    drawMenu("Search Fields", searchOptions, 5);
    cout << "Choose field: ";
    int fieldChoice = InputUtils::readIntInRange(cin, "", 1, 5);
    DeviceSearchField field = static_cast<DeviceSearchField>(fieldChoice - 1);
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
}

void Menu::sortDevices() {
    cout << "\n=== SORT DEVICES ===\n";
    string sortOptions[] = {
        "1. ID",
        "2. Device name",
        "3. Location",
        "4. Mode",
        "5. Power level"
    };
    drawMenu("Sort Fields", sortOptions, 5);
    cout << "Choose field: ";
    int fieldChoice = InputUtils::readIntInRange(cin, "", 1, 5);
    DeviceSortField field = static_cast<DeviceSortField>(fieldChoice - 1);
    
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
}

void Menu::viewDeviceInfo() {
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
}

void Menu::switchDeviceMode() {
    cout << "\n=== SWITCH DEVICE MODE ===\n";
    int id = InputUtils::readInt(cin, "Enter device ID: ");
    
    try {
        auto device = deviceService.getDeviceById(id);
        if (device == nullptr) {
            cout << "Device not found!\n";
            return;
        }
        
        string modeOptions[] = {
            "0. Automatic",
            "1. Manual",
            "2. Off"
        };
        drawMenu("Operation Modes", modeOptions, 3);
        cout << "Choose mode: ";
        int modeInt = InputUtils::readIntInRange(cin, "", 0, 2);
        device->setMode(static_cast<DeviceMode>(modeInt));
        deviceService.updateDevice(id, device);
        cout << "Mode changed successfully!\n";
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::updateDeviceStates() {
    cout << "\n=== UPDATE DEVICE STATES ===\n";
    deviceService.updateAllDevices();
    cout << "Device states updated.\n";
}

void Menu::undo() {
    if (commandHistory.canUndo()) {
        commandHistory.undo();
        cout << "Action undone.\n";
    } else {
        cout << "No actions to undo.\n";
    }
}

void Menu::redo() {
    if (commandHistory.canRedo()) {
        commandHistory.redo();
        cout << "Action redone.\n";
    } else {
        cout << "No actions to redo.\n";
    }
}

void Menu::viewEnvironmentIndicators() {
    cout << "\n=== ENVIRONMENT INDICATORS ===\n";
    cout << fixed << setprecision(2);
    cout << "Temperature: " << currentClimateData.getTemperature() << "°C";
    if (!currentClimateData.isTemperatureNormal()) cout << " (OUT OF NORMAL)";
    cout << "\n";
    cout << "Humidity: " << currentClimateData.getHumidity() << "%";
    if (!currentClimateData.isHumidityNormal()) cout << " (OUT OF NORMAL)";
    cout << "\n";
    cout << "CO2: " << currentClimateData.getCO2() << " ppm";
    if (!currentClimateData.isCO2Normal()) cout << " (OUT OF NORMAL)";
    cout << "\n";
}

void Menu::normalizeClimate() {
    cout << "\n=== NORMALIZE SMART HOME CLIMATE ===\n";
    try {
        auto devices = deviceService.getAllDevices();
        ClimateData normalized = ClimateNormalizer::normalize(currentClimateData, devices);
        
        cout << fixed << setprecision(2);
        cout << "\nInitial values:\n";
        cout << "  Temperature: " << currentClimateData.getTemperature() << "°C";
        if (!currentClimateData.isTemperatureNormal()) cout << " (OUT OF NORMAL)";
        cout << "\n";
        cout << "  Humidity: " << currentClimateData.getHumidity() << "%";
        if (!currentClimateData.isHumidityNormal()) cout << " (OUT OF NORMAL)";
        cout << "\n";
        cout << "  CO2: " << currentClimateData.getCO2() << " ppm";
        if (!currentClimateData.isCO2Normal()) cout << " (OUT OF NORMAL)";
        cout << "\n";
        
        cout << "\nNormalized values:\n";
        cout << "  Temperature: " << normalized.getTemperature() << "°C\n";
        cout << "  Humidity: " << normalized.getHumidity() << "%\n";
        cout << "  CO2: " << normalized.getCO2() << " ppm\n";
        
        string resultFilename = "normalized_climate.txt";
        ClimateFileManager::writeNormalizedToFile(resultFilename, currentClimateData, normalized);
        cout << "\nResults saved to file: " << resultFilename << "\n";
        
        currentClimateData = normalized;
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::run() {
    while (true) {
        try {
            if (!loggedIn) {
                printMainMenu();
                int choice = InputUtils::readIntInRange(cin, "", 1, 3);
                
                if (choice == 1) {
                    registerUser();
                } else if (choice == 2) {
                    login();
                } else if (choice == 3) {
                    break;
                }
            } else {
                printUserMenu();
                int choice = InputUtils::readIntInRange(cin, "", 0, 13);
                
                if (choice == 1) {
                    viewAllDevices();
                } else if (choice == 2) {
                    addDevice();
                } else if (choice == 3) {
                    editDevice();
                } else if (choice == 4) {
                    deleteDevice();
                } else if (choice == 5) {
                    searchDevices();
                } else if (choice == 6) {
                    sortDevices();
                } else if (choice == 7) {
                    viewDeviceInfo();
                } else if (choice == 8) {
                    switchDeviceMode();
                } else if (choice == 9) {
                    updateDeviceStates();
                } else if (choice == 10) {
                    undo();
                } else if (choice == 11) {
                    redo();
                } else if (choice == 12) {
                    viewEnvironmentIndicators();
                } else if (choice == 13) {
                    normalizeClimate();
                } else if (choice == 0) {
                    loggedIn = false;
                    authService.logout();
                }
            }
        } catch (const Exception& e) {
            cout << "Error: " << e.what() << "\n";
        } catch (const exception& e) {
            cout << "Unexpected error: " << e.what() << "\n";
        }
    }
}


#include "Menu.hpp"
#include "../services/include/ClimateNormalizer.hpp"
#include "../repositories/include/TextFileRepository.hpp"
#include "../models/include/devices/SmartDevice.hpp"
#include "../models/include/devices/SmartAirConditioner.hpp"
#include "../models/include/devices/SmartHeater.hpp"
#include "../models/include/devices/SmartHumidifier.hpp"
#include "../models/include/devices/SmartDehumidifier.hpp"
#include "../models/include/devices/SmartFan.hpp"
#include "../models/include/devices/SmartLight.hpp"
#include "../models/include/DeviceCommands.hpp"
#include "../exceptions/include/InputHandler.hpp"
#include "../exceptions/include/Exception.hpp"
#include "../services/include/CommandHistory.hpp"
#include <iostream>
#include <iomanip>
#include <memory>
#include <locale>
using namespace std;

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
    string fileOptions[] = {
        "1. Yes",
        "2. No"
    };
    drawMenu("Do you have a file with climate indicators?", fileOptions, 2);
    bool hasFile = false;
    bool success = false;
    while (!success) {
        try {
            int choice = 0;
            safeInputInt(cin, choice, 1, 2, "Enter choice (1-2): ");
            hasFile = (choice == 1);
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    ClimateData data;
    
    if (hasFile) {
        string filename;
        success = false;
        while (!success) {
            try {
                safeInputText(cin, filename, "Enter filename: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        try {
            TextFileRepository<ClimateData> repo(filename);
            auto dataVector = repo.readVector();
            if (!dataVector.empty()) {
                data = dataVector[0];
            } else {
                throw Exception(200, "File is empty");
            }
            cout << "\nData loaded from file: " << filename << "\n";
            cout << fixed << setprecision(2);
            cout << "Temperature: " << data.getTemperature() << "°C\n";
            cout << "Humidity: " << data.getHumidity() << "%\n";
            cout << "CO2: " << data.getCO2() << " ppm\n";
        } catch (const Exception& e) {
            cout << "Error reading file: " << e.what() << "\n";
            cout << "Please enter data manually:\n";
            double temp, hum, co2;
            success = false;
            while (!success) {
                try {
                    safeInputDouble(cin, temp, -100, 100, "Enter temperature: ");
                    success = true;
                } catch (const InputException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
            data.setTemperature(temp);
            success = false;
            while (!success) {
                try {
                    safeInputDouble(cin, hum, 0, 100, "Enter humidity: ");
                    success = true;
                } catch (const InputException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
            data.setHumidity(hum);
            success = false;
            while (!success) {
                try {
                    safeInputDouble(cin, co2, 0, 10000, "Enter CO2 level: ");
                    success = true;
                } catch (const InputException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
            data.setCO2(co2);
        }
    } else {
        cout << "Please enter climate indicators:\n";
        double temp, hum, co2;
        success = false;
        while (!success) {
            try {
                safeInputDouble(cin, temp, -100, 100, "Enter temperature: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        data.setTemperature(temp);
        success = false;
        while (!success) {
            try {
                safeInputDouble(cin, hum, 0, 100, "Enter humidity: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        data.setHumidity(hum);
        success = false;
        while (!success) {
            try {
                safeInputDouble(cin, co2, 0, 10000, "Enter CO2 level: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        data.setCO2(co2);
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
        "3. Modify device",
        "4. Delete device",
        "5. Search devices",
        "6. Sort devices",
        "7. View device information",
        "8. Undo last action",
        "9. Redo last action",
        "10. View environment indicators",
        "11. View climate recommendations",
        "12. Normalize smart home climate",
        "0. Exit"
    };
    drawMenu("MAIN MENU", userMenuOptions, 13);
    cout << "Choose an action: ";
}

void Menu::registerUser() {
    try {
        cout << "\n=== REGISTRATION ===\n";
        string username, password;
        Date birthday;
        bool success = false;
        while (!success) {
            try {
                safeInputText(cin, username, "Enter username: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        success = false;
        while (!success) {
            try {
                safeInputDate(cin, birthday, "DD/MM/YYYY", "Enter birthday: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        cout << "Enter password: ";
        getline(cin, password);
        if (password.empty()) {
            cout << "Error: Password cannot be empty.\n";
            return;
        }
        
        User newUser = authService.registerUser(username, password, birthday);
        cout << "Registration successful! User ID: " << newUser.getId() << "\n";
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::login() {
    try {
        cout << "\n=== LOGIN ===\n";
        string username, password;
        bool success = false;
        while (!success) {
            try {
                safeInputText(cin, username, "Enter username: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        cout << "Enter password: ";
        getline(cin, password);
        if (password.empty()) {
            cout << "Error: Password cannot be empty.\n";
            return;
        }
        
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
        if (!devices.empty()) {
            devices[0]->printHeader();
        }
        for (const auto& device : devices) {
            device->printTable();
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
    int deviceType = 0;
    bool success = false;
    while (!success) {
        try {
            safeInputInt(cin, deviceType, 1, 6, "");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    int id = 0;
    success = false;
    while (!success) {
        try {
            safeInputInt(cin, id, 0, 999999, "Enter device ID: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    string name, location;
    success = false;
    while (!success) {
        try {
            safeInputText(cin, name, "Enter device name: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    success = false;
    while (!success) {
        try {
            safeInputText(cin, location, "Enter location: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    string modeOptions[] = {
        "0. Automatic",
        "1. Manual",
        "2. Off"
    };
    drawMenu("Operation Modes", modeOptions, 3);
    cout << "Choose mode: ";
    int modeInt = 0;
    success = false;
    while (!success) {
        try {
            safeInputInt(cin, modeInt, 0, 2, "");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    DeviceMode mode = static_cast<DeviceMode>(modeInt);
    int power = 0;
    success = false;
    while (!success) {
        try {
            safeInputInt(cin, power, 0, 100, "Enter power level (0-100): ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
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
        int offHour = 0;
        success = false;
        while (!success) {
            try {
                safeInputInt(cin, offHour, 0, 23, "Enter turn off hour (0-23): ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
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

void Menu::modifyDevice() {
    cout << "\n=== MODIFY DEVICE ===\n";
    auto devices = deviceService.getAllDevices();
    if (devices.empty()) {
        cout << "Device list is empty. No devices to modify.\n";
        return;
    }
    
    int id = 0;
    bool success = false;
    while (!success) {
        try {
            safeInputInt(cin, id, 0, 999999, "Enter device ID: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    try {
        auto device = deviceService.getDeviceById(id);
        if (device == nullptr) {
            cout << "Device not found!\n";
            return;
        }
        
        string modifyOptions[2] = {"1. Change All Data", "2. Change Specific Field"};
        drawMenu("Modify Options", modifyOptions, 2);
        int modifyChoice = 0;
        success = false;
        while (!success) {
            try {
                safeInputInt(cin, modifyChoice, 1, 2, "Enter choice (1-2): ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        if(modifyChoice == 1) {
            SmartAirConditioner* ac = dynamic_cast<SmartAirConditioner*>(device.get());
            SmartHeater* heater = dynamic_cast<SmartHeater*>(device.get());
            SmartHumidifier* humidifier = dynamic_cast<SmartHumidifier*>(device.get());
            SmartDehumidifier* dehumidifier = dynamic_cast<SmartDehumidifier*>(device.get());
            SmartFan* fan = dynamic_cast<SmartFan*>(device.get());
            SmartLight* light = dynamic_cast<SmartLight*>(device.get());
            
            if(ac) {
                cin >> *ac;
            } else if(heater) {
                cin >> *heater;
            } else if(humidifier) {
                cin >> *humidifier;
            } else if(dehumidifier) {
                cin >> *dehumidifier;
            } else if(fan) {
                cin >> *fan;
            } else if(light) {
                cin >> *light;
            } else {
                cin >> *device;
            }
            deviceService.updateDevice(id, device);
            cout << "Device updated successfully.\n";
            return;
        }
        
        int fieldChoice = 0;
        SmartAirConditioner* ac = dynamic_cast<SmartAirConditioner*>(device.get());
        SmartHeater* heater = dynamic_cast<SmartHeater*>(device.get());
        SmartHumidifier* humidifier = dynamic_cast<SmartHumidifier*>(device.get());
        SmartDehumidifier* dehumidifier = dynamic_cast<SmartDehumidifier*>(device.get());
        SmartFan* fan = dynamic_cast<SmartFan*>(device.get());
        SmartLight* light = dynamic_cast<SmartLight*>(device.get());
        
        if(ac) {
            string fieldOptions[7] = {
                "1. Device ID", "2. Device Name", "3. Location", "4. Power Level",
                "5. Mode", "6. Target Temperature", "7. Tolerance"
            };
            drawMenu("Select Field", fieldOptions, 7);
            success = false;
            while (!success) {
                try {
                    safeInputInt(cin, fieldChoice, 1, 7, "Enter choice (1-7): ");
                    success = true;
                } catch (const InputException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
        } else if(heater) {
            string fieldOptions[7] = {
                "1. Device ID", "2. Device Name", "3. Location", "4. Power Level",
                "5. Mode", "6. Target Temperature", "7. Tolerance"
            };
            drawMenu("Select Field", fieldOptions, 7);
            success = false;
            while (!success) {
                try {
                    safeInputInt(cin, fieldChoice, 1, 7, "Enter choice (1-7): ");
                    success = true;
                } catch (const InputException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
        } else if(humidifier || dehumidifier) {
            string fieldOptions[7] = {
                "1. Device ID", "2. Device Name", "3. Location", "4. Power Level",
                "5. Mode", "6. Target Humidity", "7. Tolerance"
            };
            drawMenu("Select Field", fieldOptions, 7);
            success = false;
            while (!success) {
                try {
                    safeInputInt(cin, fieldChoice, 1, 7, "Enter choice (1-7): ");
                    success = true;
                } catch (const InputException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
        } else if(fan) {
            string fieldOptions[7] = {
                "1. Device ID", "2. Device Name", "3. Location", "4. Power Level",
                "5. Mode", "6. Target CO2", "7. Tolerance"
            };
            drawMenu("Select Field", fieldOptions, 7);
            success = false;
            while (!success) {
                try {
                    safeInputInt(cin, fieldChoice, 1, 7, "Enter choice (1-7): ");
                    success = true;
                } catch (const InputException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
        } else if(light) {
            string fieldOptions[6] = {
                "1. Device ID", "2. Device Name", "3. Location", "4. Power Level",
                "5. Mode", "6. Turn Off Hour"
            };
            drawMenu("Select Field", fieldOptions, 6);
            success = false;
            while (!success) {
                try {
                    safeInputInt(cin, fieldChoice, 1, 6, "Enter choice (1-6): ");
                    success = true;
                } catch (const InputException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
        } else {
            string fieldOptions[5] = {
                "1. Device ID", "2. Device Name", "3. Location", "4. Power Level",
                "5. Mode"
            };
            drawMenu("Select Field", fieldOptions, 5);
            success = false;
            while (!success) {
                try {
                    safeInputInt(cin, fieldChoice, 1, 5, "Enter choice (1-5): ");
                    success = true;
                } catch (const InputException& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
        }
        
        try {
            device->updateField(fieldChoice);
            deviceService.updateDevice(id, device);
            cout << "Field updated successfully.\n";
        } catch (const InputException& e) {
            cout << "Error updating field: " << e.what() << endl;
        }
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::deleteDevice() {
    cout << "\n=== DELETE DEVICE ===\n";
    auto devices = deviceService.getAllDevices();
    if (devices.empty()) {
        cout << "Device list is empty. No devices to delete.\n";
        return;
    }
    
    int id = 0;
    bool success = false;
    while (!success) {
        try {
            safeInputInt(cin, id, 0, 999999, "Enter device ID: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
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
    int fieldChoice = 0;
    bool success = false;
    while (!success) {
        try {
            safeInputInt(cin, fieldChoice, 1, 5, "");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    DeviceSearchField field = static_cast<DeviceSearchField>(fieldChoice - 1);
    string value;
    success = false;
    while (!success) {
        try {
            safeInputText(cin, value, "Enter search value: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
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
    int fieldChoice = 0;
    bool success = false;
    while (!success) {
        try {
            safeInputInt(cin, fieldChoice, 1, 5, "");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
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
    int id = 0;
    bool success = false;
    while (!success) {
        try {
            safeInputInt(cin, id, 0, 999999, "Enter device ID: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
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

void Menu::viewClimateRecommendations() {
    try {
        auto devices = deviceService.getAllDevices();
        ClimateNormalizer::analyzeAndRecommend(currentClimateData, devices);
    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::normalizeClimate() {
    string title = "CLIMATE CONTROL MODE";
    string options[] = {
        "1. Standard Normalization (Max Comfort)",
        "0. Cancel"
    };
    
    drawMenu(title, options, 2);
    
    int choice = 0;
    bool success = false;
    while (!success) {
        try {
            safeInputInt(cin, choice, 0, 1, "Select option: ");
            success = true;
        } catch (const InputException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    if (choice == 0 || choice != 1) {
        return; 
    }

    cout << "\n=== PROCESSING CLIMATE DATA ===\n";

    try {
        auto devices = deviceService.getAllDevices();
        
        ClimateData processedData = currentClimateData;
        string operationName = "Standard Normalization";
        bool changesMade = true;

        processedData = ClimateNormalizer::normalize(currentClimateData, devices);

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
        
        cout << "\nResult after " << operationName << ":\n";
        cout << "  Temperature: " << processedData.getTemperature() << "°C\n";
        cout << "  Humidity: " << processedData.getHumidity() << "%\n";
        cout << "  CO2: " << processedData.getCO2() << " ppm\n";
        
        string resultFilename = "normalized_climate.txt";
        
        TextFileRepository<ClimateData> repo(resultFilename);
        vector<ClimateData> normalizedData;
        normalizedData.push_back(processedData);
        repo.write(normalizedData);
        cout << "\nResults saved to file: " << resultFilename << "\n";
        
        currentClimateData = processedData;

    } catch (const Exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void Menu::run() {
    while (true) {
        try {
            if (!loggedIn) {
                printMainMenu();
                int choice = 0;
                bool success = false;
                while (!success) {
                    try {
                        safeInputInt(cin, choice, 1, 3, "");
                        success = true;
                    } catch (const InputException& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                }
                
                if (choice == 1) {
                    registerUser();
                } else if (choice == 2) {
                    login();
                } else if (choice == 3) {
                    break;
                }
            } else {
                printUserMenu();
                int choice = 0;
                bool success = false;
                while (!success) {
                    try {
                        safeInputInt(cin, choice, 0, 12, "");
                        success = true;
                    } catch (const InputException& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                }
                
                if (choice == 1) {
                    viewAllDevices();
                } else if (choice == 2) {
                    addDevice();
                } else if (choice == 3) {
                    modifyDevice();
                } else if (choice == 4) {
                    deleteDevice();
                } else if (choice == 5) {
                    searchDevices();
                } else if (choice == 6) {
                    sortDevices();
                } else if (choice == 7) {
                    viewDeviceInfo();
                } else if (choice == 8) {
                    undo();
                } else if (choice == 9) {
                    redo();
                } else if (choice == 10) {
                    viewEnvironmentIndicators();
                } else if (choice == 11) {
                    viewClimateRecommendations();
                } else if (choice == 12) {
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


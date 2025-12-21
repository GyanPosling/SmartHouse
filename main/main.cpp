#include "Menu.hpp"
#include "../services/include/AuthenticationService.hpp"
#include "../services/include/DeviceService.hpp"
#include "../services/include/CommandHistory.hpp"
#include "../services/include/ClimateNormalizer.hpp"
#include "../models/include/ClimateData.hpp"
#include "../repositories/include/TextFile.hpp"
#include "../models/include/devices/SmartDevice.hpp"
#include "../models/include/devices/SmartAirConditioner.hpp"
#include "../models/include/devices/SmartHeater.hpp"
#include "../models/include/devices/SmartHumidifier.hpp"
#include "../models/include/devices/SmartDehumidifier.hpp"
#include "../models/include/devices/SmartFan.hpp"
#include "../models/include/devices/SmartLight.hpp"
#include "../models/include/DeviceCommands.hpp"
#include "../models/include/RemoveDeviceCommand.hpp"
#include "../exceptions/include/InputHandler.hpp"
#include <iostream>
#include <iomanip>
#include <locale>
using namespace std;

Language currentLang = Language::ENGLISH;
AuthenticationService authService;
DeviceService deviceService;
CommandHistory commandHistory;
ClimateData currentClimateData;
bool loggedIn = false;

ClimateData loadClimateData() {
    cout << "\n=== CLIMATE DATA SETUP ===\n";
    string options[] = { "1. Yes", "2. No" };
    Menu::draw("Load climate data from file?", options, 2);

    int choice = safeInputNumeric<int>(cin, 1, 2, "Enter choice (1-2): ");
    ClimateData data;

    if (choice == 1) {
        string filename = safeGetLine(cin, Language::ENGLISH, "Enter filename: ");

        try {
            TextFile<ClimateData> file(filename);
            auto records = file.readAllRecords();

            if (!records.isEmpty()) {
                auto it = records.begin();
                data = **it;
                for (auto ptr : records) delete ptr;

                cout << fixed << setprecision(2);
                cout << "\nData successfully loaded from '" << filename << "':\n";
                cout << "Temperature: " << data.getTemperature() << " degrees C\n";
                cout << "Humidity: " << data.getHumidity() << "%\n";
                cout << "CO2: " << data.getCO2() << " ppm\n";
                return data;
            }

            for (auto ptr : records) delete ptr;
            cout << "File '" << filename << "' is empty. Switching to manual input.\n";

        } catch (const FileException& e) {
            cout << "File not found or cannot be read: " << filename << "\n";
            cout << "Switching to manual input.\n";
        }
    }

    cout << "\nEnter climate indicators manually:\n";
    double temp = safeInputNumeric<double>(cin, -100.0, 100.0, "Temperature (degrees C): ");
    double hum  = safeInputNumeric<double>(cin,  0.0,  100.0, "Humidity (%): ");
    double co2  = safeInputNumeric<double>(cin,  0.0, 10000.0, "CO2 level (ppm): ");

    data.setTemperature(temp);
    data.setHumidity(hum);
    data.setCO2(co2);

    return data;
}

void registerUser() {
    cout << "\n=== REGISTRATION ===\n";
    string username = safeGetLine(cin, Language::ENGLISH, "Enter username: ");
    Date birthday = safeInputDate(cin, "DD/MM/YYYY", "Enter birthday (DD/MM/YYYY): ");
    string password;
    cout << "Enter password: ";
    getline(cin, password);
    if (password.empty()) {
        cout << "Password cannot be empty!\n";
        return;
    }

    try {
        User user = authService.registerUser(username, password, birthday);
        cout << "Registration successful! Your ID: " << user.getId() << "\n";
    } catch (const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void loginUser() {
    cout << "\n=== LOGIN ===\n";
    string username = safeGetLine(cin, Language::ENGLISH, "Enter username: ");
    string password;
    cout << "Enter password: ";
    getline(cin, password);
    if (password.empty()) {
        cout << "Password cannot be empty!\n";
        return;
    }

    try {
        authService.loginUser(username, password);
        loggedIn = true;
        cout << "Login successful!\n";
    } catch (const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void viewAllDevices() {
    cout << "\n=== ALL DEVICES ===\n";
    auto devices = deviceService.getAllDevices();
    if (devices.isEmpty()) {
        cout << "No devices available.\n";
        return;
    }
    auto it = devices.begin();
    (*it)->printHeader();
    for (const auto& dev : devices) {
        dev->printTable();
    }
}

void addDevice() {
    cout << "\n=== ADD DEVICE ===\n";
    string typeOptions[] = {
        "1. Air Conditioner",
        "2. Heater",
        "3. Humidifier",
        "4. Dehumidifier",
        "5. Fan",
        "6. Smart Light"
    };
    Menu::draw("Select device type", typeOptions, 6);
    int deviceType = safeInputNumeric<int>(cin, 1, 6, "Enter type (1-6): ");

    int deviceId = safeInputNumeric<int>(cin, 0, 999999, "Enter device ID: ");
    string deviceName = safeGetLine(cin, Language::ENGLISH, "Enter device name: ");
    string deviceLocation = safeGetLine(cin, Language::ENGLISH, "Enter location: ");

    string modeOptions[] = { "0. Automatic", "1. Manual", "2. Off" };
    Menu::draw("Select operation mode", modeOptions, 3);
    DeviceMode operationMode = static_cast<DeviceMode>(safeInputNumeric<int>(cin, 0, 2, "Mode (0-2): "));

    int powerLevel = safeInputNumeric<int>(cin, 0, 100, "Power level (0-100): ");

    SmartDevice* newDevice = nullptr;

    const double DEFAULT_TEMP = 25.0;
    const double DEFAULT_HUM = 30.0;
    const double DEFAULT_CO2 = 400.0;
    const double DEFAULT_TOLERANCE = 5.0;

    switch (deviceType) {
        case 1: newDevice = new SmartAirConditioner(deviceId, deviceName, deviceLocation, operationMode, powerLevel, DEFAULT_TEMP, DEFAULT_TOLERANCE); break;
        case 2: newDevice = new SmartHeater(deviceId, deviceName, deviceLocation, operationMode, powerLevel, DEFAULT_TEMP, DEFAULT_TOLERANCE); break;
        case 3: newDevice = new SmartHumidifier(deviceId, deviceName, deviceLocation, operationMode, powerLevel, DEFAULT_HUM, DEFAULT_TOLERANCE); break;
        case 4: newDevice = new SmartDehumidifier(deviceId, deviceName, deviceLocation, operationMode, powerLevel, DEFAULT_HUM, DEFAULT_TOLERANCE); break;
        case 5: newDevice = new SmartFan(deviceId, deviceName, deviceLocation, operationMode, powerLevel, DEFAULT_CO2, DEFAULT_TOLERANCE); break;
        case 6:
            int turnOffHour = safeInputNumeric<int>(cin, 0, 23, "Turn off hour (0-23): ");
            newDevice = new SmartLight(deviceId, deviceName, deviceLocation, operationMode, powerLevel, turnOffHour);
            break;
    }

    try {
        commandHistory.executeCommand(new AddDeviceCommand(&deviceService, newDevice));
        cout << "Device successfully added!\n";
    } catch (const exception& e) {
        cout << "Error adding device: " << e.what() << "\n";
    }
}

void modifyDevice() {
    cout << "\n=== MODIFY DEVICE ===\n";
    auto allDevices = deviceService.getAllDevices();
    if (allDevices.isEmpty()) {
        cout << "No devices to modify.\n";
        return;
    }

    int targetId = safeInputNumeric<int>(cin, 0, 999999, "Enter device ID to modify: ");
    auto deviceToModify = deviceService.getDeviceById(targetId);
    if (!deviceToModify) {
        cout << "Device with ID " << targetId << " not found.\n";
        return;
    }

    auto cloneDevice = [](SmartDevice* device) -> SmartDevice* {
        if (auto ac = dynamic_cast<SmartAirConditioner*>(device)) {
            return new SmartAirConditioner(*ac);
        } else if (auto heater = dynamic_cast<SmartHeater*>(device)) {
            return new SmartHeater(*heater);
        } else if (auto humidifier = dynamic_cast<SmartHumidifier*>(device)) {
            return new SmartHumidifier(*humidifier);
        } else if (auto dehumidifier = dynamic_cast<SmartDehumidifier*>(device)) {
            return new SmartDehumidifier(*dehumidifier);
        } else if (auto fan = dynamic_cast<SmartFan*>(device)) {
            return new SmartFan(*fan);
        } else if (auto light = dynamic_cast<SmartLight*>(device)) {
            return new SmartLight(*light);
        }
        return new SmartDevice(*device);
    };

    string modifyOptions[] = { "1. Change all data", "2. Change specific field" };
    Menu::draw("Modification mode", modifyOptions, 2);
    int modifyMode = safeInputNumeric<int>(cin, 1, 2, "Choose (1-2): ");

    if (modifyMode == 1) {
        cin >> *deviceToModify;
        SmartDevice* updated = cloneDevice(deviceToModify);
        deviceService.updateDevice(targetId, updated);
        cout << "Device fully updated!\n";
        return;
    }

    SmartAirConditioner* ac = dynamic_cast<SmartAirConditioner*>(deviceToModify);
    SmartHeater* heater = dynamic_cast<SmartHeater*>(deviceToModify);
    SmartHumidifier* humidifier = dynamic_cast<SmartHumidifier*>(deviceToModify);
    SmartDehumidifier* dehumidifier = dynamic_cast<SmartDehumidifier*>(deviceToModify);
    SmartFan* fan = dynamic_cast<SmartFan*>(deviceToModify);
    SmartLight* light = dynamic_cast<SmartLight*>(deviceToModify);

    if (ac || heater) {
        string fields[] = {
            "1. ID", "2. Name", "3. Location", "4. Power Level",
            "5. Mode", "6. Target Temperature", "7. Tolerance"
        };
        Menu::draw("Select field to change", fields, 7);
        int field = safeInputNumeric<int>(cin, 1, 7, "Field number: ");
        deviceToModify->updateField(field);
    }
    else if (humidifier || dehumidifier) {
        string fields[] = {
            "1. ID", "2. Name", "3. Location", "4. Power Level",
            "5. Mode", "6. Target Humidity", "7. Tolerance"
        };
        Menu::draw("Select field to change", fields, 7);
        int field = safeInputNumeric<int>(cin, 1, 7, "Field number: ");
        deviceToModify->updateField(field);
    }
    else if (fan) {
        string fields[] = {
            "1. ID", "2. Name", "3. Location", "4. Power Level",
            "5. Mode", "6. Target CO2", "7. Tolerance"
        };
        Menu::draw("Select field to change", fields, 7);
        int field = safeInputNumeric<int>(cin, 1, 7, "Field number: ");
        deviceToModify->updateField(field);
    }
    else if (light) {
        string fields[] = {
            "1. ID", "2. Name", "3. Location", "4. Power Level",
            "5. Mode", "6. Turn off hour"
        };
        Menu::draw("Select field to change", fields, 6);
        int field = safeInputNumeric<int>(cin, 1, 6, "Field number: ");
        deviceToModify->updateField(field);
    }
    else {
        string fields[] = { "1. ID", "2. Name", "3. Location", "4. Power Level", "5. Mode" };
        Menu::draw("Select field to change", fields, 5);
        int field = safeInputNumeric<int>(cin, 1, 5, "Field number: ");
        deviceToModify->updateField(field);
    }

    SmartDevice* updated = cloneDevice(deviceToModify);
    deviceService.updateDevice(targetId, updated);
    cout << "Field updated successfully!\n";
}

void deleteDevice() {
    cout << "\n=== DELETE DEVICE ===\n";
    if (deviceService.getAllDevices().isEmpty()) {
        cout << "No devices to delete.\n";
        return;
    }

    int deviceId = safeInputNumeric<int>(cin, 0, 999999, "Enter device ID to delete: ");
    try {
        commandHistory.executeCommand(new RemoveDeviceCommand(&deviceService, deviceId));
        cout << "Device deleted!\n";
    } catch (const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void searchDevices() {
    cout << "\n=== SEARCH DEVICES ===\n";
    string searchOptions[] = {
        "1. By ID",
        "2. By Name",
        "3. By Location",
        "4. By Power Level",
        "5. By Mode"
    };
    Menu::draw("Search by field", searchOptions, 5);
    int searchOption = safeInputNumeric<int>(cin, 1, 5, "Choose field (1-5): ");

    auto results = deviceService.searchDevices(searchOption);

    if (results.isEmpty()) {
        cout << "No devices found.\n";
    } else {
        cout << "Found " << results.size() << " device(s):\n";
        for (const auto& dev : results) {
            cout << dev->getDeviceInfo() << "\n";
        }
    }
}

void sortDevices() {
    cout << "\n=== SORT DEVICES ===\n";
    if (deviceService.getAllDevices().isEmpty()) {
        cout << "No devices to sort.\n";
        return;
    }

    string sortOptions[] = {
        "1. By ID",
        "2. By Name",
        "3. By Location",
        "4. By Power Level",
        "5. By Mode"
    };
    Menu::draw("Sort by field", sortOptions, 5);
    int sortOption = safeInputNumeric<int>(cin, 1, 5, "Choose field (1-5): ");

    deviceService.sortDevices(sortOption);
    cout << "Devices sorted successfully.\n";
}

void viewDeviceInfo() {
    cout << "\n=== DEVICE INFORMATION ===\n";
    int deviceId = safeInputNumeric<int>(cin, 0, 999999, "Enter device ID: ");
    auto device = deviceService.getDeviceById(deviceId);
    if (device) {
        cout << device->getDeviceInfo() << "\n";
    } else {
        cout << "Device not found.\n";
    }
}

void undoLastAction() {
    if (commandHistory.canUndo()) {
        commandHistory.undo();
        cout << "Last action undone.\n";
    } else {
        cout << "Nothing to undo.\n";
    }
}

void redoLastAction() {
    if (commandHistory.canRedo()) {
        commandHistory.redo();
        cout << "Action redone.\n";
    } else {
        cout << "Nothing to redo.\n";
    }
}

void viewEnvironmentIndicators() {
    cout << "\n=== CURRENT ENVIRONMENT ===\n";
    cout << fixed << setprecision(2);
    cout << "Temperature: " << currentClimateData.getTemperature() << " degrees C"
         << (currentClimateData.isTemperatureNormal() ? "" : " (OUT OF RANGE)") << "\n";
    cout << "Humidity: " << currentClimateData.getHumidity() << "%"
         << (currentClimateData.isHumidityNormal() ? "" : " (OUT OF RANGE)") << "\n";
    cout << "CO2: " << currentClimateData.getCO2() << " ppm"
         << (currentClimateData.isCO2Normal() ? "" : " (OUT OF RANGE)") << "\n";
}

void viewClimateRecommendations() {
    cout << "\n=== CLIMATE RECOMMENDATIONS ===\n";
    ClimateNormalizer::analyzeAndRecommend(currentClimateData, deviceService.getAllDevices());
}

void normalizeClimate() {
    string options[] = { "1. Normalize (maximum comfort)", "0. Cancel" };
    Menu::draw("CLIMATE NORMALIZATION", options, 2);
    int choice = safeInputNumeric<int>(cin, 0, 1, "Your choice: ");
    if (choice != 1) return;

    cout << "\nNormalizing climate...\n";
    ClimateData normalized = ClimateNormalizer::normalize(currentClimateData, deviceService.getAllDevices());

    cout << fixed << setprecision(2);
    cout << "Normalization complete:\n";
    cout << "Temperature: " << normalized.getTemperature() << " degrees C\n";
    cout << "Humidity: " << normalized.getHumidity() << "%\n";
    cout << "CO2: " << normalized.getCO2() << " ppm\n";

    TextFile<ClimateData> resultFile("normalized_climate.txt");
    resultFile.clearFile();
    resultFile.saveRecord(normalized);
    cout << "Results saved to 'normalized_climate.txt'\n";

    currentClimateData = normalized;
}

void run(){
    currentClimateData = loadClimateData();

    while (true) {
        if (!loggedIn) {
            string mainOptions[] = { "1. Register", "2. Login", "3. Exit" };
            Menu::draw("SMART HOME", mainOptions, 3);
            int choice = safeInputNumeric<int>(cin, 1, 3, "Choose action: ");

            if (choice == 3) break;
            if (choice == 1) registerUser();
            if (choice == 2) loginUser();
        } else {
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
                "0. Logout"
            };
            Menu::draw("MAIN MENU", userMenuOptions, 13);
            int choice = safeInputNumeric<int>(cin, 0, 12, "Choose action: ");

            if (choice == 0) {
                loggedIn = false;
                authService.logout();
                continue;
            }

            switch (choice) {
                case 1: viewAllDevices(); break;
                case 2: addDevice(); break;
                case 3: modifyDevice(); break;
                case 4: deleteDevice(); break;
                case 5: searchDevices(); break;
                case 6: sortDevices(); break;
                case 7: viewDeviceInfo(); break;
                case 8: undoLastAction(); break;
                case 9: redoLastAction(); break;
                case 10: viewEnvironmentIndicators(); break;
                case 11: viewClimateRecommendations(); break;
                case 12: normalizeClimate(); break;
            }
        }
        cout << "\n";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int restart;
    do{
        run();
        restart = safeInputNumeric<int>(cin, 0, 1, "Do you want to restart project(0 - no / 1 - yes ): ");
        
    }while(restart != 0);
    return 0;
}

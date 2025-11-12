#pragma once
#include "../../services/include/AuthenticationService.hpp"
#include "../../services/include/DeviceService.hpp"
#include "../../services/include/CommandHistory.hpp"
#include "../../services/include/ClimateData.hpp"
#include <string>
using namespace std;

class Menu {
private:
    AuthenticationService authService;
    DeviceService deviceService;
    CommandHistory commandHistory;
    ClimateData currentClimateData;
    bool loggedIn;

    void printMainMenu();
    void printUserMenu();
    
    void registerUser();
    void login();
    
    void viewAllDevices();
    void addDevice();
    void editDevice();
    void deleteDevice();
    void searchDevices();
    void sortDevices();
    void viewDeviceInfo();
    void switchDeviceMode();
    void updateDeviceStates();
    void undo();
    void redo();
    void viewEnvironmentIndicators();
    void normalizeClimate();
    
    ClimateData getClimateData();

public:
    Menu();
    void run();
};


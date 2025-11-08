#include "../include/AutomationService.hpp"
#include <ctime>

AutomationService::AutomationService(shared_ptr<EnvironmentSimulator> simulator)
    : environmentSimulator(simulator) {}

void AutomationService::updateDevices(vector<shared_ptr<SmartDevice>>& devices) {
    // Обновляем показатели среды
    environmentSimulator->updateEnvironment();
    
    double temperature = environmentSimulator->getTemperature();
    double humidity = environmentSimulator->getHumidity();
    double co2 = environmentSimulator->getCO2();
    int hour = getCurrentHour();
    
    // Обновляем все устройства
    for (auto& device : devices) {
        if (device->getMode() == DeviceMode::AUTOMATIC) {
            // Только устройства в автоматическом режиме обновляются
            device->update(temperature, humidity, co2, hour);
        }
        // Устройства в ручном режиме остаются в текущем состоянии
    }
}

int AutomationService::getCurrentHour() const {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    return localTime->tm_hour;
}

double AutomationService::getCurrentTemperature() const {
    return environmentSimulator->getTemperature();
}

double AutomationService::getCurrentHumidity() const {
    return environmentSimulator->getHumidity();
}

double AutomationService::getCurrentCO2() const {
    return environmentSimulator->getCO2();
}


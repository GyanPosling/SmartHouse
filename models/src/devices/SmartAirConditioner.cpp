#include "../../include/devices/SmartAirConditioner.hpp"

SmartAirConditioner::SmartAirConditioner() 
    : SmartDevice(), targetCO2(400.0), tolerance(50.0) {}

SmartAirConditioner::SmartAirConditioner(int deviceId, const string& name, const string& location,
                                         DeviceMode deviceMode, int power, double targetCO2, double deviceTolerance)
    : SmartDevice(deviceId, name, location, deviceMode, power), 
      targetCO2(targetCO2), tolerance(deviceTolerance) {}

double SmartAirConditioner::getTargetCO2() const {
    return targetCO2;
}

void SmartAirConditioner::setTargetCO2(double target) {
    targetCO2 = target;
}

double SmartAirConditioner::getTolerance() const {
    return tolerance;
}

void SmartAirConditioner::setTolerance(double deviceTolerance) {
    tolerance = deviceTolerance;
}

void SmartAirConditioner::update(double temperature, double humidity, double co2, int hour) {
    if (mode == DeviceMode::MANUAL) {
        // В ручном режиме устройство работает пока пользователь не выключит
        return;
    }
    
    if (mode == DeviceMode::OFF) {
        turnOff();
        return;
    }
    
    // Автоматический режим - кондиционер работает для температуры
    // Кондиционер должен снижать температуру при высокой температуре
    // Используем targetCO2 как целевую температуру (для обратной совместимости)
    double targetTemp = targetCO2; // В данном случае targetCO2 используется как целевая температура
    if (temperature > targetTemp + tolerance) {
        // Температура выше нормы - включаем кондиционер
        turnOn();
    } else if (temperature <= targetTemp - tolerance) {
        // Температура в норме - выключаем
        turnOff();
    }
}

bool SmartAirConditioner::operator==(const SmartAirConditioner& other) const {
    return SmartDevice::operator==(other);
}

bool SmartAirConditioner::operator<(const SmartAirConditioner& other) const {
    return SmartDevice::operator<(other);
}

string SmartAirConditioner::getDeviceInfo() const {
    return SmartDevice::getDeviceInfo() + 
           ", Type: Air Conditioner" + 
           ", Target CO2: " + to_string(targetCO2) + " ppm" +
           ", Tolerance: ±" + to_string(tolerance) + " ppm";
}


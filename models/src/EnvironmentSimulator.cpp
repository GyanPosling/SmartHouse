#include "../include/EnvironmentSimulator.hpp"
#include <algorithm>

EnvironmentSimulator::EnvironmentSimulator() 
    : generator(static_cast<unsigned int>(time(nullptr))),
      minTemperature(15.0), maxTemperature(30.0),
      minHumidity(30.0), maxHumidity(70.0),
      minCO2(300.0), maxCO2(1000.0),
      targetTemperature(22.0), targetHumidity(50.0), targetCO2(400.0) {
    initializeSensors();
}

void EnvironmentSimulator::initializeSensors() {
    sensors.clear();
    sensors.push_back(Sensor(1, "Temperature", 20.0, "C"));
    sensors.push_back(Sensor(2, "Humidity", 45.0, "%"));
    sensors.push_back(Sensor(3, "CO2", 400.0, "ppm"));
}

void EnvironmentSimulator::updateEnvironment() {
    std::uniform_real_distribution<double> tempDist(minTemperature, maxTemperature);
    std::uniform_real_distribution<double> humidityDist(minHumidity, maxHumidity);
    std::uniform_real_distribution<double> co2Dist(minCO2, maxCO2);
    
    for (auto& sensor : sensors) {
        if (sensor.getSensorName() == "Temperature") {
            double newTemp = tempDist(generator);
            sensor.setValue(newTemp);
        } else if (sensor.getSensorName() == "Humidity") {
            double newHumidity = humidityDist(generator);
            sensor.setValue(newHumidity);
        } else if (sensor.getSensorName() == "CO2") {
            double newCO2 = co2Dist(generator);
            sensor.setValue(newCO2);
        }
    }
}

double EnvironmentSimulator::getTemperature() const {
    for (const auto& sensor : sensors) {
        if (sensor.getSensorName() == "Temperature") {
            return sensor.getValue();
        }
    }
    return 20.0;
}

double EnvironmentSimulator::getHumidity() const {
    for (const auto& sensor : sensors) {
        if (sensor.getSensorName() == "Humidity") {
            return sensor.getValue();
        }
    }
    return 45.0;
}

double EnvironmentSimulator::getCO2() const {
    for (const auto& sensor : sensors) {
        if (sensor.getSensorName() == "CO2") {
            return sensor.getValue();
        }
    }
    return 400.0;
}

void EnvironmentSimulator::setTargetTemperature(double target) {
    targetTemperature = target;
}

void EnvironmentSimulator::setTargetHumidity(double target) {
    targetHumidity = target;
}

void EnvironmentSimulator::setTargetCO2(double target) {
    targetCO2 = target;
}

double EnvironmentSimulator::getTargetTemperature() const {
    return targetTemperature;
}

double EnvironmentSimulator::getTargetHumidity() const {
    return targetHumidity;
}

double EnvironmentSimulator::getTargetCO2() const {
    return targetCO2;
}

vector<Sensor> EnvironmentSimulator::getSensors() const {
    return sensors;
}

Sensor EnvironmentSimulator::getSensorByName(const string& name) const {
    for (const auto& sensor : sensors) {
        if (sensor.getSensorName() == name) {
            return sensor;
        }
    }
    return Sensor();
}

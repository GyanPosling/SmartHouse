#pragma once
#include <vector>
#include <random>
#include <ctime>
#include "Sensor.hpp"
using namespace std;

class EnvironmentSimulator {
private:
    vector<Sensor> sensors;
    mt19937 generator;
    double minTemperature;
    double maxTemperature;
    double minHumidity;
    double maxHumidity;
    double minCO2;
    double maxCO2;
    double targetTemperature;
    double targetHumidity;
    double targetCO2;

public:
    EnvironmentSimulator();
    
    void initializeSensors();
    void updateEnvironment();
    
    double getTemperature() const;
    double getHumidity() const;
    double getCO2() const;
    
    void setTargetTemperature(double target);
    void setTargetHumidity(double target);
    void setTargetCO2(double target);
    
    double getTargetTemperature() const;
    double getTargetHumidity() const;
    double getTargetCO2() const;
    
    vector<Sensor> getSensors() const;
    Sensor getSensorByName(const string& name) const;
};

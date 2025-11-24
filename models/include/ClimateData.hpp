#pragma once

#include <string>
#include <iostream>
using namespace std;

class ClimateData {
private:
    double temperature;
    double humidity;
    double co2;
    
public:
    static constexpr double MIN_TEMP = 18.0;
    static constexpr double MAX_TEMP = 25.0;
    static constexpr double MIN_HUMIDITY = 30.0;
    static constexpr double MAX_HUMIDITY = 60.0;
    static constexpr double MIN_CO2 = 400.0;
    static constexpr double MAX_CO2 = 1000.0;
    
    ClimateData();
    ClimateData(double temp, double hum, double co2Level);
    
    double getTemperature() const;
    double getHumidity() const;
    double getCO2() const;
    
    void setTemperature(double temp);
    void setHumidity(double hum);
    void setCO2(double co2Level);
    
    bool isTemperatureNormal() const;
    bool isHumidityNormal() const;
    bool isCO2Normal() const;
    bool isAllNormal() const;
    
    friend ostream& operator<<(ostream& os, const ClimateData& data);
    friend istream& operator>>(istream& is, ClimateData& data);
};


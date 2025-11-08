#pragma once
#include <string>
using namespace std;

class Sensor {
private:
    int id;
    std::string sensorName;
    double value;
    std::string unit;

public:
    Sensor();
    Sensor(int sensorId, const std::string& name, double sensorValue, const std::string& sensorUnit);
    
    int getId() const;
    void setId(int sensorId);
    
    std::string getSensorName() const;
    void setSensorName(const std::string& name);
    
    double getValue() const;
    void setValue(double sensorValue);
    
    void updateValue(double newValue);
    
    std::string getUnit() const;
    void setUnit(const std::string& sensorUnit);
};

#include "../include/Sensor.hpp"

Sensor::Sensor() : id(0), sensorName(""), value(0.0), unit("") {}

Sensor::Sensor(int sensorId, const std::string& name, double sensorValue, const std::string& sensorUnit)
    : id(sensorId), sensorName(name), value(sensorValue), unit(sensorUnit) {}

int Sensor::getId() const {
    return id;
}

void Sensor::setId(int sensorId) {
    id = sensorId;
}

std::string Sensor::getSensorName() const {
    return sensorName;
}

void Sensor::setSensorName(const std::string& name) {
    sensorName = name;
}

double Sensor::getValue() const {
    return value;
}

void Sensor::setValue(double sensorValue) {
    value = sensorValue;
}

void Sensor::updateValue(double newValue) {
    value = newValue;
}

std::string Sensor::getUnit() const {
    return unit;
}

void Sensor::setUnit(const std::string& sensorUnit) {
    unit = sensorUnit;
}


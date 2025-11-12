#include "../include/ClimateData.hpp"

ClimateData::ClimateData() : temperature(20.0), humidity(50.0), co2(500.0) {}

ClimateData::ClimateData(double temp, double hum, double co2Level) 
    : temperature(temp), humidity(hum), co2(co2Level) {}

double ClimateData::getTemperature() const {
    return temperature;
}

double ClimateData::getHumidity() const {
    return humidity;
}

double ClimateData::getCO2() const {
    return co2;
}

void ClimateData::setTemperature(double temp) {
    temperature = temp;
}

void ClimateData::setHumidity(double hum) {
    humidity = hum;
}

void ClimateData::setCO2(double co2Level) {
    co2 = co2Level;
}

bool ClimateData::isTemperatureNormal() const {
    return temperature >= MIN_TEMP && temperature <= MAX_TEMP;
}

bool ClimateData::isHumidityNormal() const {
    return humidity >= MIN_HUMIDITY && humidity <= MAX_HUMIDITY;
}

bool ClimateData::isCO2Normal() const {
    return co2 >= MIN_CO2 && co2 <= MAX_CO2;
}

bool ClimateData::isAllNormal() const {
    return isTemperatureNormal() && isHumidityNormal() && isCO2Normal();
}


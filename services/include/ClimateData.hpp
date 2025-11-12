#pragma once

#include <string>
using namespace std;

// Класс для хранения климатических данных
class ClimateData {
private:
    double temperature;  // Температура в градусах Цельсия
    double humidity;    // Влажность в процентах
    double co2;         // Уровень CO2 в ppm
    
public:
    // Нормальные диапазоны
    static constexpr double MIN_TEMP = 18.0;
    static constexpr double MAX_TEMP = 25.0;
    static constexpr double MIN_HUMIDITY = 30.0;
    static constexpr double MAX_HUMIDITY = 60.0;
    static constexpr double MIN_CO2 = 400.0;
    static constexpr double MAX_CO2 = 1000.0;
    
    ClimateData();
    ClimateData(double temp, double hum, double co2Level);
    
    // Геттеры
    double getTemperature() const;
    double getHumidity() const;
    double getCO2() const;
    
    // Сеттеры
    void setTemperature(double temp);
    void setHumidity(double hum);
    void setCO2(double co2Level);
    
    // Проверка, находятся ли показатели в норме
    bool isTemperatureNormal() const;
    bool isHumidityNormal() const;
    bool isCO2Normal() const;
    bool isAllNormal() const;
};


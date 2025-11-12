#include "../include/ClimateNormalizer.hpp"
#include "../../models/include/devices/SmartHeater.hpp"
#include "../../models/include/devices/SmartAirConditioner.hpp"
#include "../../models/include/devices/SmartHumidifier.hpp"
#include "../../models/include/devices/SmartDehumidifier.hpp"
#include "../../models/include/devices/SmartFan.hpp"

ClimateData ClimateNormalizer::normalize(ClimateData& currentData, 
                                         vector<shared_ptr<SmartDevice>>& devices) {
    ClimateData normalized = currentData;
    
    // Фиксированные целевые значения
    const double TARGET_TEMP = 25.0;
    const double TARGET_HUMIDITY = 30.0;
    const double TARGET_CO2 = 400.0;
    
    // Проверяем наличие устройств для каждого показателя
    bool hasHeater = false;
    bool hasAirConditioner = false;
    bool hasHumidifier = false;
    bool hasDehumidifier = false;
    bool hasFan = false;
    
    for (const auto& device : devices) {
        if (dynamic_cast<SmartHeater*>(device.get())) {
            hasHeater = true;
        } else if (dynamic_cast<SmartAirConditioner*>(device.get())) {
            hasAirConditioner = true;
        } else if (dynamic_cast<SmartHumidifier*>(device.get())) {
            hasHumidifier = true;
        } else if (dynamic_cast<SmartDehumidifier*>(device.get())) {
            hasDehumidifier = true;
        } else if (dynamic_cast<SmartFan*>(device.get())) {
            hasFan = true;
        }
    }
    
    // Нормализуем температуру только если есть соответствующее устройство
    if (!normalized.isTemperatureNormal()) {
        if (normalized.getTemperature() < ClimateData::MIN_TEMP) {
            // Температура ниже нормы - нужен обогреватель
            if (hasHeater) {
                normalized.setTemperature(TARGET_TEMP);
            }
            // Если нет обогревателя - оставляем как есть
        } else if (normalized.getTemperature() > ClimateData::MAX_TEMP) {
            // Температура выше нормы - нужен кондиционер
            if (hasAirConditioner) {
                normalized.setTemperature(TARGET_TEMP);
            }
            // Если нет кондиционера - оставляем как есть
        }
    }
    
    // Нормализуем влажность только если есть соответствующее устройство
    if (!normalized.isHumidityNormal()) {
        if (normalized.getHumidity() < ClimateData::MIN_HUMIDITY) {
            // Влажность ниже нормы - нужен увлажнитель
            if (hasHumidifier) {
                normalized.setHumidity(TARGET_HUMIDITY);
            }
            // Если нет увлажнителя - оставляем как есть
        } else if (normalized.getHumidity() > ClimateData::MAX_HUMIDITY) {
            // Влажность выше нормы - нужен осушитель
            if (hasDehumidifier) {
                normalized.setHumidity(TARGET_HUMIDITY);
            }
            // Если нет осушителя - оставляем как есть
        }
    }
    
    // Нормализуем CO2 только если есть вентилятор
    if (!normalized.isCO2Normal()) {
        if (normalized.getCO2() > ClimateData::MAX_CO2) {
            // CO2 выше нормы - нужен вентилятор
            if (hasFan) {
                normalized.setCO2(TARGET_CO2);
            }
            // Если нет вентилятора - оставляем как есть
        }
        // CO2 ниже нормы обычно не требует нормализации
    }
    
    return normalized;
}

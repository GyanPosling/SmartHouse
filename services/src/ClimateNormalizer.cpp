#include "../include/ClimateNormalizer.hpp"

using namespace std;

ClimateData ClimateNormalizer::normalize(ClimateData& currentData, const Deque<SmartDevice*>& devices) {
    ClimateData normalized = currentData;
    
    const double TARGET_TEMP_MIN = 21.0;
    const double TARGET_TEMP_MAX = 24.0;
    const double TARGET_HUMIDITY_MIN = 40.0;
    const double TARGET_HUMIDITY_MAX = 60.0;
    const double TARGET_CO2_MAX = 800.0;

    bool hasHeater = false;
    bool hasAC = false;
    bool hasHumidifier = false;
    bool hasDehumidifier = false;
    bool hasFan = false;

    for (const auto& device : devices) {
        if (dynamic_cast<SmartHeater*>(device)) hasHeater = true;
        else if (dynamic_cast<SmartAirConditioner*>(device)) hasAC = true;
        else if (dynamic_cast<SmartHumidifier*>(device)) hasHumidifier = true;
        else if (dynamic_cast<SmartDehumidifier*>(device)) hasDehumidifier = true;
        else if (dynamic_cast<SmartFan*>(device)) hasFan = true;
    }

    if (normalized.getTemperature() < TARGET_TEMP_MIN && hasHeater) {
        normalized.setTemperature((TARGET_TEMP_MIN + TARGET_TEMP_MAX) / 2.0);
    } 
    else if (normalized.getTemperature() > TARGET_TEMP_MAX && hasAC) {
        normalized.setTemperature((TARGET_TEMP_MIN + TARGET_TEMP_MAX) / 2.0);
    }

    if (normalized.getHumidity() < TARGET_HUMIDITY_MIN && hasHumidifier) {
        normalized.setHumidity((TARGET_HUMIDITY_MIN + TARGET_HUMIDITY_MAX) / 2.0);
    } 
    else if (normalized.getHumidity() > TARGET_HUMIDITY_MAX && hasDehumidifier) {
        normalized.setHumidity((TARGET_HUMIDITY_MIN + TARGET_HUMIDITY_MAX) / 2.0);
    }

    if (normalized.getCO2() > TARGET_CO2_MAX && hasFan) {
        normalized.setCO2(TARGET_CO2_MAX - 100.0);
    }

    return normalized;
}

void ClimateNormalizer::printClimateStatus(const ClimateData& currentData) {
    double temp = currentData.getTemperature();
    double humidity = currentData.getHumidity();
    double co2 = currentData.getCO2();
    
    cout << "Current climate status:\n";
    cout << "  Temperature: " << fixed << setprecision(2) << temp << "°C";
    if (!currentData.isTemperatureNormal()) {
        cout << " (OUT OF NORMAL: " << ClimateData::MIN_TEMP << "-" << ClimateData::MAX_TEMP << "°C)";
    }
    cout << "\n";
    cout << "  Humidity: " << humidity << "%";
    if (!currentData.isHumidityNormal()) {
        cout << " (OUT OF NORMAL: " << ClimateData::MIN_HUMIDITY << "-" << ClimateData::MAX_HUMIDITY << "%)";
    }
    cout << "\n";
    cout << "  CO2: " << co2 << " ppm";
    if (!currentData.isCO2Normal()) {
        cout << " (OUT OF NORMAL: " << ClimateData::MIN_CO2 << "-" << ClimateData::MAX_CO2 << " ppm)";
    }
    cout << "\n\n";
}

bool ClimateNormalizer::analyzeAndRecommend(ClimateData& currentData, const Deque<SmartDevice*>& devices) {
    bool hasRecommendations = false;
    Deque<string> recommendations;
    
    double temp = currentData.getTemperature();
    double humidity = currentData.getHumidity();
    double co2 = currentData.getCO2();
    
    bool hasHeater = false;
    bool hasAC = false;
    bool hasHumidifier = false;
    bool hasDehumidifier = false;
    bool hasFan = false;
    
    for (const auto& device : devices) {
        if (dynamic_cast<SmartHeater*>(device)) hasHeater = true;
        if (dynamic_cast<SmartAirConditioner*>(device)) hasAC = true;
        if (dynamic_cast<SmartHumidifier*>(device)) hasHumidifier = true;
        if (dynamic_cast<SmartDehumidifier*>(device)) hasDehumidifier = true;
        if (dynamic_cast<SmartFan*>(device)) hasFan = true;
    }
    
    cout << "\n=== CLIMATE ANALYSIS & RECOMMENDATIONS ===\n\n";
    printClimateStatus(currentData);
    
    if (currentData.isAllNormal()) {
        cout << "✓ All climate parameters are within normal ranges. No action needed.\n";
        return false;
    }
    
    cout << "RECOMMENDATIONS:\n\n";
    
    if (temp < ClimateData::MIN_TEMP) {
        if (hasHeater) {
            cout << "!!! Temperature is too low (" << temp << "°C). ";
            cout << "Recommendation: Turn on the heater to raise temperature to " 
                 << (ClimateData::MIN_TEMP + ClimateData::MAX_TEMP) / 2.0 << "°C.\n";
            hasRecommendations = true;
        } else {
            cout << "!!! Temperature is too low (" << temp << "°C). ";
            cout << "Recommendation: Install a heater device.\n";
            hasRecommendations = true;
        }
    } else if (temp > ClimateData::MAX_TEMP) {
        if (hasAC) {
            cout << "!!! Temperature is too high (" << temp << "°C). ";
            cout << "Recommendation: Turn on the air conditioner to lower temperature to " 
                 << (ClimateData::MIN_TEMP + ClimateData::MAX_TEMP) / 2.0 << "°C.\n";
            hasRecommendations = true;
        } else {
            cout << "!!! Temperature is too high (" << temp << "°C). ";
            cout << "Recommendation: Install an air conditioner device.\n";
            hasRecommendations = true;
        }
    }
    
    if (humidity < ClimateData::MIN_HUMIDITY) {
        if (hasHumidifier) {
            cout << "!!! Humidity is too low (" << humidity << "%). ";
            cout << "Recommendation: Turn on the humidifier to raise humidity to " 
                 << (ClimateData::MIN_HUMIDITY + ClimateData::MAX_HUMIDITY) / 2.0 << "%.\n";
            hasRecommendations = true;
        } else {
            cout << "!!! Humidity is too low (" << humidity << "%). ";
            cout << "Recommendation: Install a humidifier device.\n";
            hasRecommendations = true;
        }
    } else if (humidity > ClimateData::MAX_HUMIDITY) {
        if (hasDehumidifier) {
            cout << "!!! Humidity is too high (" << humidity << "%). ";
            cout << "Recommendation: Turn on the dehumidifier to lower humidity to " 
                 << (ClimateData::MIN_HUMIDITY + ClimateData::MAX_HUMIDITY) / 2.0 << "%.\n";
            hasRecommendations = true;
        } else {
            cout << "!!! Humidity is too high (" << humidity << "%). ";
            cout << "Recommendation: Install a dehumidifier device.\n";
            hasRecommendations = true;
        }
    }
    
    if (co2 > ClimateData::MAX_CO2) {
        if (hasFan) {
            cout << "!!! CO2 level is too high (" << co2 << " ppm). ";
            cout << "Recommendation: Turn on the fan to improve air circulation and reduce CO2 to below " 
                 << ClimateData::MAX_CO2 << " ppm.\n";
            hasRecommendations = true;
        } else {
            cout << "!!! CO2 level is too high (" << co2 << " ppm). ";
            cout << "Recommendation: Install a fan device and ensure proper ventilation.\n";
            hasRecommendations = true;
        }
    } else if (co2 < ClimateData::MIN_CO2) {
        cout << "!!! CO2 level is unusually low (" << co2 << " ppm). ";
        cout << "This may indicate a measurement error. Check your sensors.\n";
        hasRecommendations = true;
    }
    
    if (!hasRecommendations) {
        cout << "No critical issues detected. Climate is acceptable.\n";
    }
    
    cout << "\n";
    return hasRecommendations;
}

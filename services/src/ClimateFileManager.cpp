#include "../include/ClimateFileManager.hpp"
#include "../../exceptions/include/FileException.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>

ClimateData ClimateFileManager::readFromFile(const string& filename) {
    validateFile(filename);
    
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileException(200, "Cannot open file: " + filename);
    }
    
    ClimateData data;
    string line;
    
    // Читаем температуру
    if (!getline(file, line)) {
        throw FileException(201, "Invalid file format: missing temperature");
    }
    try {
        data.setTemperature(stod(line));
    } catch (...) {
        throw FileException(202, "Invalid temperature value in file");
    }
    
    // Читаем влажность
    if (!getline(file, line)) {
        throw FileException(203, "Invalid file format: missing humidity");
    }
    try {
        data.setHumidity(stod(line));
    } catch (...) {
        throw FileException(204, "Invalid humidity value in file");
    }
    
    // Читаем CO2
    if (!getline(file, line)) {
        throw FileException(205, "Invalid file format: missing CO2");
    }
    try {
        data.setCO2(stod(line));
    } catch (...) {
        throw FileException(206, "Invalid CO2 value in file");
    }
    
    file.close();
    return data;
}

void ClimateFileManager::writeToFile(const string& filename, const ClimateData& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw FileException(207, "Cannot create file: " + filename);
    }
    
    file << fixed << setprecision(2);
    file << data.getTemperature() << "\n";
    file << data.getHumidity() << "\n";
    file << data.getCO2() << "\n";
    
    file.close();
}

void ClimateFileManager::writeNormalizedToFile(const string& filename, 
                                               const ClimateData& original, 
                                               const ClimateData& normalized) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw FileException(208, "Cannot create file: " + filename);
    }
    
    file << fixed << setprecision(2);
    file << "=== CLIMATE NORMALIZATION REPORT ===\n\n";
    file << "ORIGINAL VALUES:\n";
    file << "Temperature: " << original.getTemperature() << "°C";
    if (!original.isTemperatureNormal()) {
        file << " (OUT OF NORMAL RANGE: " << ClimateData::MIN_TEMP 
             << "-" << ClimateData::MAX_TEMP << "°C)";
    }
    file << "\n";
    
    file << "Humidity: " << original.getHumidity() << "%";
    if (!original.isHumidityNormal()) {
        file << " (OUT OF NORMAL RANGE: " << ClimateData::MIN_HUMIDITY 
             << "-" << ClimateData::MAX_HUMIDITY << "%)";
    }
    file << "\n";
    
    file << "CO2: " << original.getCO2() << " ppm";
    if (!original.isCO2Normal()) {
        file << " (OUT OF NORMAL RANGE: " << ClimateData::MIN_CO2 
             << "-" << ClimateData::MAX_CO2 << " ppm)";
    }
    file << "\n\n";
    
    file << "NORMALIZED VALUES:\n";
    file << "Temperature: " << normalized.getTemperature() << "°C\n";
    file << "Humidity: " << normalized.getHumidity() << "%\n";
    file << "CO2: " << normalized.getCO2() << " ppm\n\n";
    
    file << "All values are now within normal ranges.\n";
    
    file.close();
}

void ClimateFileManager::validateFile(const string& filename) {
    if (filename.empty()) {
        throw FileException(209, "Filename cannot be empty");
    }
    
    ifstream file(filename);
    if (!file.good()) {
        throw FileException(210, "File does not exist or cannot be read: " + filename);
    }
    file.close();
}

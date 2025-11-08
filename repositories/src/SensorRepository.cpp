#include "../include/SensorRepository.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

SensorRepository::SensorRepository(const string& file) : filename(file) {}

void SensorRepository::saveSensor(const Sensor& sensor) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        throw FileException(70, "Failed to open file " + filename + " for writing");
    }
    
    file << sensor.getId() << "|" << sensor.getSensorName() << "|" 
         << sensor.getValue() << "|" << sensor.getUnit() << "\n";
    file.close();
}

vector<Sensor> SensorRepository::loadAllSensors() {
    vector<Sensor> sensors;
    ifstream file(filename);
    
    if (!file.is_open()) {
        return sensors;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        string token;
        vector<string> tokens;
        
        while (getline(iss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 4) {
            int id = stoi(tokens[0]);
            string name = tokens[1];
            double value = stod(tokens[2]);
            string unit = tokens[3];
            sensors.push_back(Sensor(id, name, value, unit));
        }
    }
    
    file.close();
    return sensors;
}

Sensor SensorRepository::findSensorById(int sensorId) {
    auto sensors = loadAllSensors();
    for (const auto& sensor : sensors) {
        if (sensor.getId() == sensorId) {
            return sensor;
        }
    }
    throw FileException(71, "Sensor with ID " + to_string(sensorId) + " not found");
}

void SensorRepository::updateSensor(const Sensor& updatedSensor) {
    auto sensors = loadAllSensors();
    bool found = false;
    
    for (auto& sensor : sensors) {
        if (sensor.getId() == updatedSensor.getId()) {
            sensor = updatedSensor;
            found = true;
            break;
        }
    }
    
    if (!found) {
        throw FileException(72, "Sensor with ID " + to_string(updatedSensor.getId()) + " not found for update");
    }
    
    // Перезаписываем файл
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) {
        throw FileException(73, "Failed to open file " + filename + " for update");
    }
    
    for (const auto& sensor : sensors) {
        file << sensor.getId() << "|" << sensor.getSensorName() << "|" 
             << sensor.getValue() << "|" << sensor.getUnit() << "\n";
    }
    
    file.close();
}

void SensorRepository::deleteSensor(int sensorId) {
    auto sensors = loadAllSensors();
    auto it = remove_if(sensors.begin(), sensors.end(),
        [sensorId](const Sensor& s) { return s.getId() == sensorId; });
    
    if (it == sensors.end()) {
        throw FileException(74, "Sensor with ID " + to_string(sensorId) + " not found for deletion");
    }
    
    sensors.erase(it, sensors.end());
    
    // Перезаписываем файл
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) {
        throw FileException(75, "Failed to open file " + filename + " for deletion");
    }
    
    for (const auto& sensor : sensors) {
        file << sensor.getId() << "|" << sensor.getSensorName() << "|" 
             << sensor.getValue() << "|" << sensor.getUnit() << "\n";
    }
    
    file.close();
}


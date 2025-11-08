#pragma once
#include "../../models/include/Sensor.hpp"
#include "../../exceptions/include/FileException.hpp"
#include <vector>
#include <string>
using namespace std;

class SensorRepository {
private:
    string filename;

public:
    SensorRepository(const string& file);
    
    void saveSensor(const Sensor& sensor);
    vector<Sensor> loadAllSensors();
    Sensor findSensorById(int sensorId);
    void updateSensor(const Sensor& sensor);
    void deleteSensor(int sensorId);
};


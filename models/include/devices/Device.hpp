#pragma once
#include<string>
#include"User.hpp"
#include"Sensor.hpp"
using namespace std;

class Device{
protected:
    int id;
    User user;
    Sensor sensor;
    string deviceName;
    string location;
    int powerLevel;
    string mode;
};
#pragma once
#include"devices/Device.hpp"
#include"User.hpp"
#include <memory>
#include<vector>
using namespace std;

class SmartHome{
private:
    int id; 
    User user;
    vector<Device> devices;
};
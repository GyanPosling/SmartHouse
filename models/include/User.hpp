#pragma once

#include<string>
#include<vector>
#include<memory>
#include"devices/Device.hpp"
using namespace std;

class User{
private:
    int id;
    string username;
    string password;
    vector<Device> userDevices;
};
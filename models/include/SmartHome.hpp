#pragma once
#include "User.hpp"
#include "devices/SmartDevice.hpp"
#include <vector>
#include <memory>
using namespace std;

class SmartHome {
private:
    int id;
    User user;
    vector<shared_ptr<SmartDevice>> devices;

public:
    SmartHome();
    SmartHome(int homeId, const User& homeUser);
    
    int getId() const;
    void setId(int homeId);
    
    User getUser() const;
    void setUser(const User& homeUser);
    
    void addDevice(shared_ptr<SmartDevice> device);
    void removeDevice(int deviceId);
    shared_ptr<SmartDevice> getDeviceById(int deviceId) const;
    vector<shared_ptr<SmartDevice>> getDevices() const;
    
    void updateAllDevices(double temperature, double humidity, double co2, int hour);
};

#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
using namespace std;

enum class DeviceSearchField {
    ID,
    DEVICE_NAME,
    LOCATION,
    MODE,
    POWER_LEVEL
};

enum class DeviceSortField {
    ID,
    DEVICE_NAME,
    LOCATION,
    MODE,
    POWER_LEVEL
};

enum class DeviceMode {
    AUTOMATIC,
    MANUAL,
    OFF
};

class Device {
protected:
    int id;
    string deviceName;
    string location;
    int powerLevel;
    bool isOn;
    
    static DeviceSearchField currentSearchMode;

public:
    Device();
    Device(int deviceId, const string& name, const string& deviceLocation, int power);
    Device(const Device& other);
    Device& operator=(const Device& other);
    virtual ~Device() = default;
    
    static void setSearchMode(DeviceSearchField mode);
    static DeviceSearchField getSearchMode();
    
    int getId() const;
    void setId(int deviceId);
    
    string getDeviceName() const;
    void setDeviceName(const string& name);
    
    string getLocation() const;
    void setLocation(const string& deviceLocation);
    
    int getPowerLevel() const;
    void setPowerLevel(int power);
    
    bool getIsOn() const;
    void setIsOn(bool on);
    
    virtual void turnOn();
    virtual void turnOff();
    virtual string getDeviceInfo() const;
    
    bool operator==(const Device& other) const;
    bool operator!=(const Device& other) const;
    bool operator<(const Device& other) const;
    bool operator>(const Device& other) const;
    bool operator<=(const Device& other) const;
    bool operator>=(const Device& other) const;
    
    friend ostream& operator<<(ostream& os, const Device& device);
    friend istream& operator>>(istream& is, Device& device);
    
    virtual void printHeader() const;
    virtual void printTable() const;
    virtual void updateField(int fieldChoice);
};
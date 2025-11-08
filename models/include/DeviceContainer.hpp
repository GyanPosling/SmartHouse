#pragma once
#include "devices/SmartDevice.hpp"
#include "../../exceptions/include/ContainerException.hpp"
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

class DeviceContainer {
private:
    vector<shared_ptr<SmartDevice>> devices;

public:
    class Iterator {
    private:
        vector<shared_ptr<SmartDevice>>::iterator it;

    public:
        Iterator(vector<shared_ptr<SmartDevice>>::iterator iterator);
        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);
        shared_ptr<SmartDevice>& operator*();
        shared_ptr<SmartDevice>* operator->();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };

    DeviceContainer();
    ~DeviceContainer() = default;
    
    void addDevice(shared_ptr<SmartDevice> device);
    void removeDevice(int deviceId);
    shared_ptr<SmartDevice> findDeviceById(int deviceId) const;
    
    vector<shared_ptr<SmartDevice>> searchByField(DeviceSearchField field, const string& value) const;
    void sortByField(DeviceSortField field);
    
    size_t size() const;
    bool empty() const;
    void clear();
    
    Iterator begin();
    Iterator end();
    
    shared_ptr<SmartDevice> operator[](size_t index) const;
};

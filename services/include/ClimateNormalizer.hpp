#pragma once

#include "ClimateData.hpp"
#include "../../models/include/devices/SmartDevice.hpp"
#include <vector>
#include <memory>
using namespace std;

class ClimateNormalizer {
public:
    // Нормализация климатических условий с использованием устройств
    // Нормализует только те показатели, для которых есть соответствующие устройства
    static ClimateData normalize(ClimateData& currentData, 
                                 vector<shared_ptr<SmartDevice>>& devices);
};

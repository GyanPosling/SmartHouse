#pragma once

#include "../models/include/ClimateData.hpp"
#include "../../models/include/devices/SmartDevice.hpp"
#include "../../templates/Deque.hpp"
#include "../../models/include/devices/SmartHeater.hpp"
#include "../../models/include/devices/SmartAirConditioner.hpp"
#include "../../models/include/devices/SmartHumidifier.hpp"
#include "../../models/include/devices/SmartDehumidifier.hpp"
#include "../../models/include/devices/SmartFan.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class ClimateNormalizer {
private:
    static void printClimateStatus(const ClimateData& currentData);

public:
    static ClimateData normalize(ClimateData& currentData, 
                                 const Deque<SmartDevice*>& devices);

    static bool analyzeAndRecommend(ClimateData& currentData, const Deque<SmartDevice*>& devices);
};

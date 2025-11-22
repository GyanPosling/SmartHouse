#pragma once

#include "ClimateData.hpp"
#include "../../models/include/devices/SmartDevice.hpp"
#include <vector>
#include <memory>
using namespace std;

class ClimateNormalizer {
private:
    static void printClimateStatus(const ClimateData& currentData);

public:
    static ClimateData normalize(ClimateData& currentData, 
                                 vector<shared_ptr<SmartDevice>>& devices);

    static bool analyzeAndRecommend(ClimateData& currentData, vector<shared_ptr<SmartDevice>>& devices);
};

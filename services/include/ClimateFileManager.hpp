#pragma once

#include "ClimateData.hpp"
#include "../../exceptions/include/FileException.hpp"
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class ClimateFileManager {
public:
    // Чтение климатических данных из файла
    static ClimateData readFromFile(const string& filename);
    
    // Запись климатических данных в файл
    static void writeToFile(const string& filename, const ClimateData& data);
    
    // Запись нормализованных данных в файл с подробной информацией
    static void writeNormalizedToFile(const string& filename, const ClimateData& original, 
                                      const ClimateData& normalized);
    
private:
    static void validateFile(const string& filename);
};

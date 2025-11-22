#pragma once
#include <fstream>
#include <string>
#include "../../exceptions/include/FileException.hpp"
using namespace std;

class FileRepository {
protected:
    string filename;
    ifstream inputFile;
    ofstream outputFile;
    fstream fileStream;

public:
    FileRepository(const string& file);
    virtual ~FileRepository();
    
    bool fileExists() const;
    void openForRead();
    void openForWrite();
    void openForAppend();
    void close();
    
    string getFilename() const;
    void setFilename(const string& file);
};


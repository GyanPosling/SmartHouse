#pragma once
#include "Exception.hpp"
using namespace std;

class FileException : public Exception {
public:
    FileException(int errorCode, const string& errorMessage);
};



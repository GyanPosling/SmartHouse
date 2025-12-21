#pragma once
#include "Exception.hpp"
using namespace std;

class InputException : public Exception {
public:
    InputException(int errorCode, const string& errorMessage);
};

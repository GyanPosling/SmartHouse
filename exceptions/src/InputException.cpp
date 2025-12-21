#include "../include/InputException.hpp"
using namespace std;

InputException::InputException(int errorCode, const string& errorMessage)
    : Exception(errorCode, errorMessage) {}



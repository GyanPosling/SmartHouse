#include "../include/FileException.hpp"
using namespace std;

FileException::FileException(int errorCode, const string& errorMessage)
    : Exception(errorCode, errorMessage) {}



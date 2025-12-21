#include "../include/Exception.hpp"
using namespace std;

Exception::Exception(int errorCode, const string& errorMessage) 
    : code(errorCode), message(errorMessage) {}

int Exception::getCode() const {
    return code;
}

const char* Exception::what() const noexcept {
    return message.c_str();
}



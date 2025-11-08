#pragma once
#include <string>
#include <exception>
using namespace std;

class Exception : public exception {
protected:
    int code;
    string message;

public:
    Exception(int errorCode, const string& errorMessage);
    virtual ~Exception() = default;
    
    int getCode() const;
    const char* what() const noexcept override;
};


#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include "../../exceptions/include/Date.hpp"
using namespace std;

class User {
protected:
    int id;
    string username;
    string password;
    Date birthday;

public:
    User();
    User(int userId, const string& name, const string& userPassword);
    User(int userId, const string& name, const string& userPassword, const Date& userBirthday);
    virtual ~User();
    
    User& operator=(const User& other);
    bool operator==(const User& other) const;
    bool operator<(const User& other) const;
    friend ostream& operator<<(ostream& os, const User& user);
    friend istream& operator>>(istream& is, User& user);
    
    int getId() const;
    void setId(int userId);
    
    string getUsername() const;
    void setUsername(const string& name);
    
    string getPassword() const;
    void setPassword(const string& userPassword);
    
    Date getBirthday() const;
    void setBirthday(const Date& userBirthday);
    
    bool checkPassword(const string& userPassword) const;
    
    virtual void printHeader() const;
    virtual void printTable() const;
};

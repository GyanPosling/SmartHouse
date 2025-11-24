#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include "../../exceptions/include/Date.hpp"

class User {
protected:
    int id;
    std::string username;
    std::string password;
    Date birthday;

public:
    User();
    User(int userId, const std::string& name, const std::string& userPassword);
    User(int userId, const std::string& name, const std::string& userPassword, const Date& userBirthday);
    virtual ~User();
    
    User& operator=(const User& other);
    bool operator==(const User& other) const;
    bool operator<(const User& other) const;
    friend std::ostream& operator<<(std::ostream& os, const User& user);
    friend std::istream& operator>>(std::istream& is, User& user);
    
    int getId() const;
    void setId(int userId);
    
    std::string getUsername() const;
    void setUsername(const std::string& name);
    
    std::string getPassword() const;
    void setPassword(const std::string& userPassword);
    
    Date getBirthday() const;
    void setBirthday(const Date& userBirthday);
    
    bool checkPassword(const std::string& userPassword) const;
    
    virtual void printHeader() const;
    virtual void printTable() const;
};
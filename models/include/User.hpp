#pragma once
#include <string>
using namespace std;

class User {
private:
    int id;
    std::string username;
    std::string password;

public:
    User();
    User(int userId, const std::string& name, const std::string& userPassword);
    
    int getId() const;
    void setId(int userId);
    
    std::string getUsername() const;
    void setUsername(const std::string& name);
    
    std::string getPassword() const;
    void setPassword(const std::string& userPassword);
    
    bool checkPassword(const std::string& userPassword) const;
};

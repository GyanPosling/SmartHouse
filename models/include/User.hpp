#pragma once
#include <string>
using namespace std;

class User {
private:
    int id;
    std::string username;
    std::string password;
    std::string birthday; // Формат: DD.MM.YYYY

public:
    User();
    User(int userId, const std::string& name, const std::string& userPassword);
    User(int userId, const std::string& name, const std::string& userPassword, const std::string& userBirthday);
    
    int getId() const;
    void setId(int userId);
    
    std::string getUsername() const;
    void setUsername(const std::string& name);
    
    std::string getPassword() const;
    void setPassword(const std::string& userPassword);
    
    std::string getBirthday() const;
    void setBirthday(const std::string& userBirthday);
    
    bool checkPassword(const std::string& userPassword) const;
};

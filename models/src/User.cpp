#include "../include/User.hpp"

User::User() : id(0), username(""), password("") {}

User::User(int userId, const std::string& name, const std::string& userPassword)
    : id(userId), username(name), password(userPassword) {}

int User::getId() const {
    return id;
}

void User::setId(int userId) {
    id = userId;
}

std::string User::getUsername() const {
    return username;
}

void User::setUsername(const std::string& name) {
    username = name;
}

std::string User::getPassword() const {
    return password;
}

void User::setPassword(const std::string& userPassword) {
    password = userPassword;
}

bool User::checkPassword(const std::string& userPassword) const {
    return password == userPassword;
}


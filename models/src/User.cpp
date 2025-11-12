#include "../include/User.hpp"

User::User() : id(0), username(""), password(""), birthday("") {}

User::User(int userId, const std::string& name, const std::string& userPassword)
    : id(userId), username(name), password(userPassword), birthday("") {}

User::User(int userId, const std::string& name, const std::string& userPassword, const std::string& userBirthday)
    : id(userId), username(name), password(userPassword), birthday(userBirthday) {}

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

std::string User::getBirthday() const {
    return birthday;
}

void User::setBirthday(const std::string& userBirthday) {
    birthday = userBirthday;
}

bool User::checkPassword(const std::string& userPassword) const {
    return password == userPassword;
}


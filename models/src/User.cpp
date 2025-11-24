#include "../include/User.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <sstream>
#include <vector>
#include <stdexcept>

User::User() : id(0), username(""), password(""), birthday() {}

User::User(int userId, const std::string& name, const std::string& userPassword)
    : id(userId), username(name), password(userPassword), birthday() {}

User::User(int userId, const std::string& name, const std::string& userPassword, const Date& userBirthday)
    : id(userId), username(name), password(userPassword), birthday(userBirthday) {}

User::~User() {}

User& User::operator=(const User& other) {
    if (this != &other) {
        this->id = other.id;
        this->username = other.username;
        this->password = other.password;
        this->birthday = other.birthday;
    }
    return *this;
}

bool User::operator==(const User& other) const {
    return this->id == other.id &&
           this->username == other.username &&
           this->password == other.password &&
           this->birthday == other.birthday;
}

bool User::operator<(const User& other) const {
    return this->id < other.id;
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os << user.id << "|" << user.username << "|" << user.password << "|" << user.birthday << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, User& user) {
    bool isCin = (&is == &std::cin);
    
    if (isCin) {
        bool success = false;
        while (!success) {
            try {
                user.id = safeInputNumeric<int>(is, 0, 999999, "Enter user ID: ");
                success = true;
            } catch (const InputException& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
        
        success = false;
        while (!success) {
            try {
                user.username = safeGetLine(is, Language::ENGLISH, "Enter username: ");
                success = true;
            } catch (const InputException& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
        
        std::cout << "Enter password: ";
        std::string passwordInput;
        std::getline(is, passwordInput);
        if (passwordInput.empty()) {
            throw InputException(18, "Password cannot be empty.");
        }
        user.password = passwordInput;
        
        success = false;
        while (!success) {
            try {
                user.birthday = safeInputDate(is, "DD/MM/YYYY", "Enter birthday: ");
                success = true;
            } catch (const InputException& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    } else {
        std::string line;
        if (std::getline(is, line)) {
            if (line.empty()) {
                is.setstate(std::ios::failbit);
                return is;
            }
            
            std::stringstream ss(line);
            std::string idStr, username, password, birthdayStr;
            
            if (std::getline(ss, idStr, '|') &&
                std::getline(ss, username, '|') &&
                std::getline(ss, password, '|') &&
                std::getline(ss, birthdayStr))
            {
                try {
                    user.id = std::stoi(idStr);
                    user.username = username;
                    user.password = password;
                    
                    if (!birthdayStr.empty()) {
                        std::stringstream dateStream(birthdayStr);
                        int day, month, year;
                        char separator1, separator2;
                        
                        if (dateStream >> day >> separator1 >> month >> separator2 >> year) {
                            if (separator1 == '/' && separator2 == '/') {
                                user.birthday = Date(day, month, year);
                            }
                        }
                    } else {
                        user.birthday = Date();
                    }
                } catch (const std::exception&) {
                    is.setstate(std::ios::failbit);
                }
            } else {
                is.setstate(std::ios::failbit);
            }
        }
    }
    
    return is;
}

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

Date User::getBirthday() const {
    return birthday;
}

void User::setBirthday(const Date& userBirthday) {
    birthday = userBirthday;
}

bool User::checkPassword(const std::string& userPassword) const {
    return password == userPassword;
}

void User::printHeader() const {
    std::cout << std::left;
    std::cout << "| " << std::setw(5) << "ID" << " | " << std::setw(15) << "Username" << " | " << std::setw(15) << "Password" << " | " << std::setw(12) << "Birthday" << " |" << std::endl;
}

void User::printTable() const {
    std::cout << std::left;
    std::cout << "| " << std::setw(5) << id << " | " << std::setw(15) << username << " | " << std::setw(15) << password << " | " << std::setw(12) << birthday << " |" << std::endl;
}
#include "../include/User.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <sstream>
#include <stdexcept>
using namespace std;

User::User() : id(0), username(""), password(""), birthday() {}

User::User(int userId, const string& name, const string& userPassword)
    : id(userId), username(name), password(userPassword), birthday() {}

User::User(int userId, const string& name, const string& userPassword, const Date& userBirthday)
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

ostream& operator<<(ostream& os, const User& user) {
    os << user.id << "|" << user.username << "|" << user.password << "|" << user.birthday << "\n";
    return os;
}

istream& operator>>(istream& is, User& user) {
    bool isCin = (&is == &cin);
    
    if (isCin) {
        bool success = false;
        while (!success) {
            try {
                user.id = safeInputNumeric<int>(is, 0, 999999, "Enter user ID: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        success = false;
        while (!success) {
            try {
                user.username = safeGetLine(is, Language::ENGLISH, "Enter username: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        cout << "Enter password: ";
        string passwordInput;
        getline(is, passwordInput);
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
                cout << "Error: " << e.what() << endl;
            }
        }
    } else {
        string line;
        if (getline(is, line)) {
            if (line.empty()) {
                is.setstate(ios::failbit);
                return is;
            }
            
            stringstream ss(line);
            string idStr, username, password, birthdayStr;
            
            if (getline(ss, idStr, '|') &&
                getline(ss, username, '|') &&
                getline(ss, password, '|') &&
                getline(ss, birthdayStr))
            {
                try {
                    user.id = stoi(idStr);
                    user.username = username;
                    user.password = password;
                    
                    if (!birthdayStr.empty()) {
                        stringstream dateStream(birthdayStr);
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
                } catch (const exception&) {
                    is.setstate(ios::failbit);
                }
            } else {
                is.setstate(ios::failbit);
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

string User::getUsername() const {
    return username;
}

void User::setUsername(const string& name) {
    username = name;
}

string User::getPassword() const {
    return password;
}

void User::setPassword(const string& userPassword) {
    password = userPassword;
}

Date User::getBirthday() const {
    return birthday;
}

void User::setBirthday(const Date& userBirthday) {
    birthday = userBirthday;
}

bool User::checkPassword(const string& userPassword) const {
    return password == userPassword;
}

void User::printHeader() const {
    cout << left;
    cout << "| " << setw(5) << "ID" << " | " << setw(15) << "Username" << " | " << setw(15) << "Password" << " | " << setw(12) << "Birthday" << " |" << endl;
}

void User::printTable() const {
    cout << left;
    cout << "| " << setw(5) << id << " | " << setw(15) << username << " | " << setw(15) << password << " | " << setw(12) << birthday << " |" << endl;
}


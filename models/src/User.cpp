#include "../include/User.hpp"
#include "../../exceptions/include/InputHandler.hpp"
#include <sstream>
#include <vector>
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
    os << user.id << "|" << user.username << "|" << user.password << "|" << user.birthday;
    return os;
}

istream& operator>>(istream& is, User& user) {
    bool isCin = (&is == &cin);
    
    if (isCin) {
        bool success = false;
        while (!success) {
            try {
                safeInputInt(is, user.id, 0, 999999, "Enter user ID: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        
        success = false;
        while (!success) {
            try {
                safeInputText(is, user.username, "Enter username: ");
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
                safeInputDate(is, user.birthday, "DD/MM/YYYY", "Enter birthday: ");
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
            istringstream iss(line);
            string token;
            vector<string> tokens;
            
            while (getline(iss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() >= 3) {
                try {
                    user.id = stoi(tokens[0]);
                    user.username = tokens[1];
                    user.password = tokens[2];
                    if (tokens.size() >= 4 && !tokens[3].empty()) {
                        istringstream dateStream(tokens[3]);
                        char separator1, separator2;
                        int day, month, year;
                        if (dateStream >> day >> separator1 >> month >> separator2 >> year) {
                            if (separator1 == '/' && separator2 == '/') {
                                user.birthday = Date(day, month, year);
                            }
                        }
                    }
                } catch (const exception&) {
                    is.setstate(ios::failbit);
                    return is;
                }
            } else {
                is.setstate(ios::failbit);
                return is;
            }
        } else {
            is.setstate(ios::failbit);
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


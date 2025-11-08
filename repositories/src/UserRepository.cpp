#include "../include/UserRepository.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

UserRepository::UserRepository(const string& file) : filename(file) {}

void UserRepository::saveUser(const User& user) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        throw FileException(50, "Failed to open file " + filename + " for writing");
    }
    
    file << user.getId() << "|" << user.getUsername() << "|" << user.getPassword() << "\n";
    file.close();
}

vector<User> UserRepository::loadAllUsers() {
    vector<User> users;
    ifstream file(filename);
    
    if (!file.is_open()) {
        // File may not exist on first run
        return users;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        string token;
        vector<string> tokens;
        
        while (getline(iss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 3) {
            int id = stoi(tokens[0]);
            string username = tokens[1];
            string password = tokens[2];
            users.push_back(User(id, username, password));
        }
    }
    
    file.close();
    return users;
}

User UserRepository::findUserById(int userId) {
    vector<User> users = loadAllUsers();
    for (const auto& user : users) {
        if (user.getId() == userId) {
            return user;
        }
    }
    throw FileException(51, "User with ID " + to_string(userId) + " not found");
}

User UserRepository::findUserByUsername(const string& username) {
    vector<User> users = loadAllUsers();
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            return user;
        }
    }
    throw FileException(52, "User with name " + username + " not found");
}

bool UserRepository::userExists(const string& username) {
    try {
        findUserByUsername(username);
        return true;
    } catch (const FileException&) {
        return false;
    }
}

void UserRepository::updateUser(const User& updatedUser) {
    std::vector<User> users = loadAllUsers();
    bool found = false;
    
    for (auto& user : users) {
        if (user.getId() == updatedUser.getId()) {
            user = updatedUser;
            found = true;
            break;
        }
    }
    
    if (!found) {
        throw FileException(53, "User with ID " + to_string(updatedUser.getId()) + " not found for update");
    }
    
    // Rewrite file
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) {
        throw FileException(54, "Failed to open file " + filename + " for update");
    }
    
    for (const auto& user : users) {
        file << user.getId() << "|" << user.getUsername() << "|" << user.getPassword() << "\n";
    }
    
    file.close();
}

void UserRepository::deleteUser(int userId) {
    vector<User> users = loadAllUsers();
    auto it = remove_if(users.begin(), users.end(),
        [userId](const User& u) { return u.getId() == userId; });
    
    if (it == users.end()) {
        throw FileException(55, "User with ID " + to_string(userId) + " not found for deletion");
    }
    
    users.erase(it, users.end());
    
    // Rewrite file
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) {
        throw FileException(56, "Failed to open file " + filename + " for deletion");
    }
    
    for (const auto& user : users) {
        file << user.getId() << "|" << user.getUsername() << "|" << user.getPassword() << "\n";
    }
    
    file.close();
}


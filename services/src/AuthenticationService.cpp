#include "../include/AuthenticationService.hpp"

AuthenticationService::AuthenticationService() 
    : userFile("users.txt"), nextUserId(1), loggedIn(false) {
    try {
        auto users = userFile.readAllRecords();
        if (!users.isEmpty()) {
            int maxId = 0;
            for (const auto& user : users) {
                if (user->getId() > maxId) {
                    maxId = user->getId();
                }
                delete user;
            }
            nextUserId = maxId + 1;
        }
    } catch (...) {
    }
}

User AuthenticationService::registerUser(const string& username, const string& password) {
    auto users = userFile.readAllRecords();
    for (const auto& user : users) {
        if (user->getUsername() == username) {
            delete user;
            throw FileException(57, "User with name " + username + " already exists");
        }
        delete user;
    }
    
    User newUser(nextUserId, username, password);
    userFile.saveRecord(newUser);
    nextUserId++;
    
    return newUser;
}

User AuthenticationService::registerUser(const string& username, const string& password, const Date& birthday) {
    auto users = userFile.readAllRecords();
    for (const auto& user : users) {
        if (user->getUsername() == username) {
            delete user;
            throw FileException(57, "User with name " + username + " already exists");
        }
        delete user;
    }
    
    User newUser(nextUserId, username, password, birthday);
    userFile.saveRecord(newUser);
    nextUserId++;
    
    return newUser;
}

User AuthenticationService::loginUser(const string& username, const string& password) {
    auto users = userFile.readAllRecords();
    for (const auto& user : users) {
        if (user->getUsername() == username) {
            if (!user->checkPassword(password)) {
                delete user;
                throw FileException(58, "Invalid password");
            }
            currentUser = *user;
            loggedIn = true;
            delete user;
            return currentUser;
        }
        delete user;
    }
    throw FileException(52, "User with name " + username + " not found");
}

bool AuthenticationService::isUserLoggedIn() const {
    return loggedIn;
}

User AuthenticationService::getCurrentUser() const {
    if (!loggedIn) {
        throw FileException(59, "User not authorized");
    }
    return currentUser;
}

void AuthenticationService::logout() {
    loggedIn = false;
    currentUser = User();
}

int AuthenticationService::getNextUserId() {
    return nextUserId++;
}

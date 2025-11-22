#include "../include/AuthenticationService.hpp"

AuthenticationService::AuthenticationService() 
    : userRepository(make_unique<TextFileRepository<User>>("users.txt")), 
      nextUserId(1), loggedIn(false) {
    try {
        auto users = userRepository->readVector();
        if (!users.empty()) {
            int maxId = 0;
            for (const auto& user : users) {
                if (user.getId() > maxId) {
                    maxId = user.getId();
                }
            }
            nextUserId = maxId + 1;
        }
    } catch (...) {
    }
}

User AuthenticationService::registerUser(const string& username, const string& password) {
    auto users = userRepository->readVector();
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            throw FileException(57, "User with name " + username + " already exists");
        }
    }
    
    User newUser(nextUserId, username, password);
    userRepository->append(newUser);
    nextUserId++;
    
    return newUser;
}

User AuthenticationService::registerUser(const string& username, const string& password, const Date& birthday) {
    auto users = userRepository->readVector();
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            throw FileException(57, "User with name " + username + " already exists");
        }
    }
    
    User newUser(nextUserId, username, password, birthday);
    userRepository->append(newUser);
    nextUserId++;
    
    return newUser;
}

User AuthenticationService::loginUser(const string& username, const string& password) {
    auto users = userRepository->readVector();
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            if (!user.checkPassword(password)) {
                throw FileException(58, "Invalid password");
            }
            currentUser = user;
            loggedIn = true;
            return user;
        }
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


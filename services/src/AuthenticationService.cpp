#include "../include/AuthenticationService.hpp"
#include "../exceptions/include/FileException.hpp"

AuthenticationService::AuthenticationService() 
    : userRepository(make_unique<UserRepository>("users.txt")), 
      nextUserId(1), loggedIn(false) {
    // Determine next available ID
    try {
        auto users = userRepository->loadAllUsers();
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
        // Ignore errors on first run
    }
}

User AuthenticationService::registerUser(const string& username, const string& password) {
    if (userRepository->userExists(username)) {
        throw FileException(57, "User with name " + username + " already exists");
    }
    
    User newUser(nextUserId, username, password);
    userRepository->saveUser(newUser);
    nextUserId++;
    
    return newUser;
}

User AuthenticationService::loginUser(const string& username, const string& password) {
    User user = userRepository->findUserByUsername(username);
    
    if (!user.checkPassword(password)) {
        throw FileException(58, "Invalid password");
    }
    
    currentUser = user;
    loggedIn = true;
    return user;
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


#pragma once
#include "../../models/include/User.hpp"
#include "../../repositories/include/UserRepository.hpp"
#include "../../utils/include/InputUtils.hpp"
#include <memory>
using namespace std;

class AuthenticationService {
private:
    unique_ptr<UserRepository> userRepository;
    int nextUserId;

public:
    AuthenticationService();
    
    User registerUser(const string& username, const string& password);
    User loginUser(const string& username, const string& password);
    bool isUserLoggedIn() const;
    User getCurrentUser() const;
    void logout();
    
private:
    User currentUser;
    bool loggedIn;
    int getNextUserId();
};


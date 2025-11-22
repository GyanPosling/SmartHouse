#pragma once
#include "../../models/include/User.hpp"
#include "../../repositories/include/TextFileRepository.hpp"
#include "../../exceptions/include/Date.hpp"
#include "../../exceptions/include/FileException.hpp"
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;

class AuthenticationService {
private:
    unique_ptr<TextFileRepository<User>> userRepository;
    int nextUserId;

public:
    AuthenticationService();
    
    User registerUser(const string& username, const string& password);
    User registerUser(const string& username, const string& password, const Date& birthday);
    User loginUser(const string& username, const string& password);
    bool isUserLoggedIn() const;
    User getCurrentUser() const;
    void logout();
    
private:
    User currentUser;
    bool loggedIn;
    int getNextUserId();
};


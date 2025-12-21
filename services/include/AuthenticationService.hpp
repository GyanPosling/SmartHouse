#pragma once
#include "../../models/include/User.hpp"
#include "../../repositories/include/TextFile.hpp"
#include "../../exceptions/include/Date.hpp"
#include "../../exceptions/include/FileException.hpp"
using namespace std;

class AuthenticationService {
private:
    TextFile<User> userFile;
    int nextUserId;
    User currentUser;
    bool loggedIn;
    int getNextUserId();

public:
    AuthenticationService();
    
    User registerUser(const string& username, const string& password);
    User registerUser(const string& username, const string& password, const Date& birthday);
    User loginUser(const string& username, const string& password);
    bool isUserLoggedIn() const;
    User getCurrentUser() const;
    void logout();
    
};

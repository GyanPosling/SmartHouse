#pragma once
#include "../../models/include/User.hpp"
#include "../../exceptions/include/FileException.hpp"
#include <vector>
#include <string>
using namespace std;

class UserRepository {
private:
    string filename;

public:
    UserRepository(const string& file);
    
    void saveUser(const User& user);
    vector<User> loadAllUsers();
    User findUserById(int userId);
    User findUserByUsername(const string& username);
    bool userExists(const string& username);
    void updateUser(const User& user);
    void deleteUser(int userId);
};


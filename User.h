// Base User Class
#pragma once
#include <string>
#include <iostream>
#include "HospitalSystem.h"
using namespace std;
class User
{
public:
    string userID;
    string name;
    string password; // Store hashed
    string role;     // "doctor", "patient", "admin"

    User() : role("user") {}
    User(string id, string n, string pwd, string r) : userID(id), name(n), password(hashPassword(pwd)), role(r) {}

    virtual ~User() {}
    virtual void displayMenu() = 0;
    bool verifyPassword(string inputPassword)
    {
        return hashPassword(inputPassword) == password;
    }
    void changePassword()
    {
        string oldPwd, newPwd;
        cout << "Enter current password: ";
        cin >> oldPwd;
        if (!verifyPassword(oldPwd))
        {
            cout << "Incorrect current password!" << endl;
            return;
        }
        cout << "Enter new password: ";
        cin >> newPwd;
        password = hashPassword(newPwd);
        cout << "Password changed successfully!" << endl;
        HospitalSystem::instance->logAudit("Password changed", userID);
    }
};
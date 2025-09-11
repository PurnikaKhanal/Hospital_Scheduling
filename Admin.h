#pragma once
#include <iostream>
#include <string>
#include "User.h"
using namespace std;
class Admin : public User
{
public:
    Admin() : User() {}
    Admin(string id, string n, string pwd) : User(id, n, pwd, "admin") {}

    void addDoctor();
    void addPatient();
    void generateReports();
    void manageEmergencyOverrides();
    void displayMenu() override;
};
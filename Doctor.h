#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "user.h"
using namespace std;
class Doctor : public User
{
public:
    string specialization;
    vector<string> availableSlots; // e.g., "2025-09-11 10:00"
    bool onEmergencyDuty = false;

    Doctor() : User() {}
    Doctor(string id, string n, string pwd, string spec) : User(id, n, pwd, "doctor"), specialization(spec) {}

    void viewAppointments();
    void markEmergency();
    void updateAvailability();
    void viewPatientHistory(string patientID);
    void displayMenu() override;
};
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "User.h"
using namespace std;
class Patient : public User
{
public:
    string medicalHistory;
    vector<string> appointmentIDs;

    Patient() : User() {}
    Patient(string id, string n, string pwd, string history = "") : User(id, n, pwd, "patient"), medicalHistory(history) {}

    void bookAppointment();
    void cancelAppointment();
    void viewMedicalRecords();
    void requestEmergency();
    void displayMenu() override;
};
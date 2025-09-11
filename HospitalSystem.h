// HospitalSystem class definition
#pragma once
#include <vector>
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Admin.h"
#include "User.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// Declare the global instance pointer
extern class HospitalSystem* g_HospitalSystemInstance;

class HospitalSystem
{
public:
    vector<Doctor> doctors;
    vector<Patient> patients;
    vector<Appointment> appointments;
    vector<Admin> admins;
    static HospitalSystem *instance;

    HospitalSystem()
    {
        instance = this;
        g_HospitalSystemInstance = this;
    }

    void loadFromFile();
    void saveToFile();
    void backupData();
    void logAudit(string action, string userID);
    Appointment *findAppointment(string apptID);
    bool isSlotAvailable(string doctorID, string dateTime);
    User *authenticateUser(string userID, string password);
    Doctor *findDoctor(string doctorID);
    Patient *findPatient(string patientID);
};
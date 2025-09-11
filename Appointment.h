// Appointment class definition
#pragma once
#include <string>
#include <iostream>
using namespace std;
class Appointment
{
public:
    string apptID;
    string doctorID;
    string patientID;
    string dateTime; // ISO 8601 format
    string status;   // "scheduled", "completed", "cancelled", "emergency-cancelled"
    bool isEmergency = false;

    void reschedule();
    void cancel(string reason);
    void display() const
    {
        cout << "Appointment ID: " << apptID << ", Doctor: " << doctorID
             << ", Patient: " << patientID << ", Time: " << dateTime
             << ", Status: " << status << ", Emergency: " << (isEmergency ? "Yes" : "No") << endl;
    }
};
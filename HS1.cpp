#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <limits>
#include "Appointment.h"
#include "Doctor.h"
#include "Patient.h"
#include "Admin.h"
#include "HospitalSystem.h"
using namespace std;

// Forward declarations
class HospitalSystem;
class User;
class Doctor;
class Patient;
class Admin;
class Appointment;

// Global instance
extern HospitalSystem *g_HospitalSystemInstance;

// Utility function for hashing passwords (simple implementation)
string hashPassword(const string &password)
{
    // In a real system, use a proper hashing algorithm like bcrypt
    hash<string> hasher;
    return to_string(hasher(password));
}

// Initialize static member
HospitalSystem *HospitalSystem::instance = nullptr;
HospitalSystem *g_HospitalSystemInstance = nullptr;

// Main function
int main()
{
    srand(time(0)); // Seed for random numbers

    HospitalSystem hospital;
    hospital.loadFromFile();

    int choice;
    do
    {
        cout << "\n=== HOSPITAL SCHEDULING SYSTEM ===\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string userID, password;
            cout << "Enter User ID: ";
            cin >> userID;
            cout << "Enter Password: ";
            cin >> password;

            User *user = hospital.authenticateUser(userID, password);
            if (user)
            {
                hospital.logAudit("Logged in", userID);
                user->displayMenu();
                hospital.logAudit("Logged out", userID);
            }
            else
            {
                cout << "Invalid credentials!" << endl;
            }
        }
        else if (choice != 2)
        {
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 2);

    hospital.saveToFile();
    cout << "Goodbye!" << endl;

    return 0;
}
// Implementation of Admin methods
#include "Admin.h"
#include "HospitalSystem.h"

void Admin::addDoctor()
{
    string id, name, specialization, password;
    cout << "Enter Doctor ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Specialization: ";
    getline(cin, specialization);
    cout << "Enter Password: ";
    cin >> password;

    HospitalSystem::instance->doctors.push_back(Doctor(id, name, password, specialization));
    cout << "Doctor added successfully." << endl;
    HospitalSystem::instance->logAudit("Added doctor: " + id, userID);
}

void Admin::addPatient()
{
    string id, name, medicalHistory, password;
    cout << "Enter Patient ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Medical History: ";
    getline(cin, medicalHistory);
    cout << "Enter Password: ";
    cin >> password;

    HospitalSystem::instance->patients.push_back(Patient(id, name, password, medicalHistory));
    cout << "Patient added successfully." << endl;
    HospitalSystem::instance->logAudit("Added patient: " + id, userID);
}

void Admin::generateReports()
{
    cout << "=== SYSTEM REPORT ===\n";
    cout << "Doctors: " << HospitalSystem::instance->doctors.size() << endl;
    cout << "Patients: " << HospitalSystem::instance->patients.size() << endl;
    cout << "Appointments: " << HospitalSystem::instance->appointments.size() << endl;

    int scheduled = 0, completed = 0, cancelled = 0, emergency = 0;
    for (const auto &appt : HospitalSystem::instance->appointments)
    {
        if (appt.status == "scheduled")
            scheduled++;
        if (appt.status == "completed")
            completed++;
        if (appt.status == "cancelled" || appt.status == "patient-cancelled" ||
            appt.status == "emergency-cancelled")
            cancelled++;
        if (appt.isEmergency)
            emergency++;
    }

    cout << "  Scheduled: " << scheduled << endl;
    cout << "  Completed: " << completed << endl;
    cout << "  Cancelled: " << cancelled << endl;
    cout << "  Emergency: " << emergency << endl;

    HospitalSystem::instance->logAudit("Generated report", userID);
}

void Admin::manageEmergencyOverrides()
{
    string doctorID;
    cout << "Enter Doctor ID to mark emergency duty: ";
    cin >> doctorID;

    Doctor *doctor = HospitalSystem::instance->findDoctor(doctorID);
    if (doctor)
    {
        doctor->markEmergency();
        cout << "Doctor " << doctorID << " marked for emergency duty." << endl;
    }
    else
    {
        cout << "Doctor not found." << endl;
    }
}

void Admin::displayMenu()
{
    int choice;
    do
    {
        cout << "\n=== ADMIN MENU ===\n";
        cout << "1. Add Doctor\n";
        cout << "2. Add Patient\n";
        cout << "3. Generate Reports\n";
        cout << "4. Manage Emergency Overrides\n";
        cout << "5. Backup Data\n";
        cout << "6. Change Password\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addDoctor();
            break;
        case 2:
            addPatient();
            break;
        case 3:
            generateReports();
            break;
        case 4:
            manageEmergencyOverrides();
            break;
        case 5:
            HospitalSystem::instance->backupData();
            break;
        case 6:
            changePassword();
            break;
        case 0:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}
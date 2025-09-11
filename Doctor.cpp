// Implementation of Doctor methods
#include "Doctor.h"
#include "HospitalSystem.h"
void Doctor::viewAppointments()
{
    cout << "Appointments for Dr. " << name << ":\n";
    bool found = false;
    for (const auto &appt : HospitalSystem::instance->appointments)
    {
        if (appt.doctorID == userID)
        {
            appt.display();
            found = true;
        }
    }
    if (!found)
    {
        cout << "No appointments found." << endl;
    }
}

void Doctor::markEmergency()
{
    onEmergencyDuty = true;
    // Cancel all non-emergency appointments for today
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char today[11];
    strftime(today, sizeof(today), "%Y-%m-%d", ltm);

    int cancelledCount = 0;
    for (auto &appt : HospitalSystem::instance->appointments)
    {
        if (appt.doctorID == userID && appt.dateTime.substr(0, 10) == string(today) &&
            appt.status == "scheduled" && !appt.isEmergency)
        {
            appt.cancel("emergency-cancelled");
            cancelledCount++;
            // In a real system, notify patient here
        }
    }

    cout << "Doctor " << name << " is now on emergency duty. "
         << cancelledCount << " non-emergency appointments for today have been cancelled." << endl;
    HospitalSystem::instance->logAudit("Marked emergency duty", userID);
}

void Doctor::updateAvailability()
{
    string slot;
    cout << "Enter new available slot (YYYY-MM-DD HH:MM): ";
    cin.ignore();
    getline(cin, slot);
    availableSlots.push_back(slot);
    cout << "Availability updated." << endl;
    HospitalSystem::instance->logAudit("Updated availability", userID);
}

void Doctor::viewPatientHistory(string patientID)
{
    Patient *patient = HospitalSystem::instance->findPatient(patientID);
    if (patient)
    {
        cout << "Medical History for Patient " << patient->name << ":\n"
             << patient->medicalHistory << endl;
        HospitalSystem::instance->logAudit("Viewed patient history: " + patientID, userID);
    }
    else
    {
        cout << "Patient not found." << endl;
    }
}

void Doctor::displayMenu()
{
    int choice;
    do
    {
        cout << "\n=== DOCTOR MENU ===\n";
        cout << "1. View Appointments\n";
        cout << "2. Update Availability\n";
        cout << "3. Mark Emergency Duty\n";
        cout << "4. View Patient History\n";
        cout << "5. Change Password\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            viewAppointments();
            break;
        case 2:
            updateAvailability();
            break;
        case 3:
            markEmergency();
            break;
        case 4:
        {
            string patientID;
            cout << "Enter Patient ID: ";
            cin >> patientID;
            viewPatientHistory(patientID);
            break;
        }
        case 5:
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

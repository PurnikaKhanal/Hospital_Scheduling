// Implementation of Patient methods
#include "Patient.h"
#include "HospitalSystem.h"
void Patient::bookAppointment()
{
    string doctorID, dateTime;
    cout << "Enter Doctor ID: ";
    cin >> doctorID;

    // Check if doctor exists
    Doctor *doctor = HospitalSystem::instance->findDoctor(doctorID);
    if (!doctor)
    {
        cout << "Doctor not found!" << endl;
        return;
    }

    cout << "Enter desired appointment date and time (YYYY-MM-DD HH:MM): ";
    cin.ignore();
    getline(cin, dateTime);

    // Check doctor's availability
    if (HospitalSystem::instance->isSlotAvailable(doctorID, dateTime))
    {
        Appointment newAppt;
        newAppt.apptID = to_string(rand() % 100000); // Simple random ID
        newAppt.doctorID = doctorID;
        newAppt.patientID = userID;
        newAppt.dateTime = dateTime;
        newAppt.status = "scheduled";

        HospitalSystem::instance->appointments.push_back(newAppt);
        appointmentIDs.push_back(newAppt.apptID);

        cout << "Appointment booked successfully with ID: " << newAppt.apptID << endl;
        HospitalSystem::instance->logAudit("Booked appointment: " + newAppt.apptID, userID);
    }
    else
    {
        cout << "Selected slot is not available. Please choose another time." << endl;
    }
}

void Patient::cancelAppointment()
{
    string aptID;
    cout << "Enter Appointment ID to cancel: ";
    cin >> aptID;

    Appointment *appt = HospitalSystem::instance->findAppointment(aptID);
    if (appt && appt->patientID == userID && appt->status == "scheduled")
    {
        appt->cancel("patient-cancelled");
        cout << "Appointment cancelled successfully." << endl;
    }
    else
    {
        cout << "Appointment not found or cannot be cancelled." << endl;
    }
}

void Patient::viewMedicalRecords()
{
    cout << "Medical History: " << medicalHistory << endl;
    cout << "Appointments: " << endl;

    bool found = false;
    for (const auto &apptID : appointmentIDs)
    {
        Appointment *appt = HospitalSystem::instance->findAppointment(apptID);
        if (appt)
        {
            appt->display();
            found = true;
        }
    }

    if (!found)
    {
        cout << "No appointments found." << endl;
    }

    HospitalSystem::instance->logAudit("Viewed medical records", userID);
}

void Patient::requestEmergency()
{
    cout << "EMERGENCY REQUESTED!\n";
    cout << "Finding available doctors...\n";

    // Find available doctors
    vector<Doctor *> availableDoctors;
    for (auto &doctor : HospitalSystem::instance->doctors)
    {
        if (doctor.onEmergencyDuty)
        {
            availableDoctors.push_back(&doctor);
        }
    }

    if (availableDoctors.empty())
    {
        cout << "No doctors available for emergency right now. Please try again later.\n";
        return;
    }

    // Create emergency appointment with first available doctor
    Appointment emergencyAppt;
    emergencyAppt.apptID = "EMG-" + to_string(rand() % 10000);
    emergencyAppt.doctorID = availableDoctors[0]->userID;
    emergencyAppt.patientID = userID;

    // Set current time as appointment time
    time_t now = time(0);
    char dt[20];
    strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M", localtime(&now));
    emergencyAppt.dateTime = dt;
    emergencyAppt.status = "scheduled";
    emergencyAppt.isEmergency = true;

    HospitalSystem::instance->appointments.push_back(emergencyAppt);
    appointmentIDs.push_back(emergencyAppt.apptID);

    cout << "Emergency appointment created with Dr. " << availableDoctors[0]->name
         << ". Appointment ID: " << emergencyAppt.apptID << endl;

    HospitalSystem::instance->logAudit("Requested emergency appointment", userID);
}

void Patient::displayMenu()
{
    int choice;
    do
    {
        cout << "\n=== PATIENT MENU ===\n";
        cout << "1. Book Appointment\n";
        cout << "2. Cancel Appointment\n";
        cout << "3. View Medical Records\n";
        cout << "4. Request Emergency\n";
        cout << "5. Change Password\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            bookAppointment();
            break;
        case 2:
            cancelAppointment();
            break;
        case 3:
            viewMedicalRecords();
            break;
        case 4:
            requestEmergency();
            break;
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

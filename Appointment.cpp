// Implementation of Appointment methods
#include "Appointment.h"
#include "HospitalSystem.h"

void Appointment::reschedule()
{
    string newDateTime;
    cout << "Enter new date and time (YYYY-MM-DD HH:MM): ";
    cin.ignore();
    getline(cin, newDateTime);

    // Check if the new slot is available
    if (HospitalSystem::instance->isSlotAvailable(doctorID, newDateTime))
    {
        dateTime = newDateTime;
        cout << "Appointment rescheduled to " << dateTime << endl;
        HospitalSystem::instance->logAudit("Appointment rescheduled: " + apptID, patientID);
    }
    else
    {
        cout << "Selected slot is not available. Please choose another time." << endl;
    }
}

void Appointment::cancel(string reason)
{
    status = reason;
    HospitalSystem::instance->logAudit("Appointment cancelled: " + apptID + " Reason: " + reason, patientID);
}
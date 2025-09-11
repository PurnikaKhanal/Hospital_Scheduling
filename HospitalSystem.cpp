// Implementation of HospitalSystem methods
#include "HospitalSystem.h"
void HospitalSystem::loadFromFile()
{
    // Load doctors
    ifstream docFile("doctors.txt");
    if (docFile.is_open())
    {
        string line;
        while (getline(docFile, line))
        {
            istringstream iss(line);
            string id, name, spec, pwd;
            if (getline(iss, id, '|') && getline(iss, name, '|') &&
                getline(iss, spec, '|') && getline(iss, pwd))
            {
                doctors.push_back(Doctor(id, name, pwd, spec));
            }
        }
        docFile.close();
    }

    // Load patients
    ifstream patFile("patients.txt");
    if (patFile.is_open())
    {
        string line;
        while (getline(patFile, line))
        {
            istringstream iss(line);
            string id, name, history, pwd;
            if (getline(iss, id, '|') && getline(iss, name, '|') &&
                getline(iss, history, '|') && getline(iss, pwd))
            {
                patients.push_back(Patient(id, name, pwd, history));
            }
        }
        patFile.close();
    }

    // Load appointments
    ifstream apptFile("appointments.txt");
    if (apptFile.is_open())
    {
        string line;
        while (getline(apptFile, line))
        {
            istringstream iss(line);
            Appointment appt;
            string emergencyFlag;
            if (getline(iss, appt.apptID, '|') && getline(iss, appt.doctorID, '|') &&
                getline(iss, appt.patientID, '|') && getline(iss, appt.dateTime, '|') &&
                getline(iss, appt.status, '|') && getline(iss, emergencyFlag))
            {
                appt.isEmergency = (emergencyFlag == "1");
                appointments.push_back(appt);
            }
        }
        apptFile.close();
    }

    // Load admins (simple implementation)
    admins.push_back(Admin("admin1", "System Administrator", "admin123"));

    cout << "Data loaded successfully." << endl;
}

void HospitalSystem::saveToFile()
{
    // Save doctors
    ofstream docFile("doctors.txt");
    for (const auto &doc : doctors)
    {
        docFile << doc.userID << "|" << doc.name << "|" << doc.specialization << "|" << doc.password << endl;
    }
    docFile.close();

    // Save patients
    ofstream patFile("patients.txt");
    for (const auto &pat : patients)
    {
        patFile << pat.userID << "|" << pat.name << "|" << pat.medicalHistory << "|" << pat.password << endl;
    }
    patFile.close();

    // Save appointments
    ofstream apptFile("appointments.txt");
    for (const auto &appt : appointments)
    {
        apptFile << appt.apptID << "|" << appt.doctorID << "|" << appt.patientID << "|"
                 << appt.dateTime << "|" << appt.status << "|" << (appt.isEmergency ? "1" : "0") << endl;
    }
    apptFile.close();

    cout << "Data saved successfully." << endl;
}

void HospitalSystem::backupData()
{
    // Create backup with timestamp
    time_t now = time(0);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", localtime(&now));

    string backupDir = "backup_" + string(timestamp) + "/";
    system(("mkdir " + backupDir).c_str());

    system(("copy doctors.txt " + backupDir + "doctors.txt").c_str());
    system(("copy patients.txt " + backupDir + "patients.txt").c_str());
    system(("copy appointments.txt " + backupDir + "appointments.txt").c_str());
    system(("copy audit_log.txt " + backupDir + "audit_log.txt").c_str());

    cout << "Data backup completed to directory: " << backupDir << endl;
    logAudit("Data backup created", "system");
}

void HospitalSystem::logAudit(string action, string userID)
{
    ofstream auditFile("audit_log.txt", ios::app);
    time_t now = time(0);
    char dt[30];
    strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));
    auditFile << dt << " | User: " << userID << " | Action: " << action << endl;
    auditFile.close();
}

Appointment *HospitalSystem::findAppointment(string apptID)
{
    for (auto &appt : appointments)
    {
        if (appt.apptID == apptID)
        {
            return &appt;
        }
    }
    return nullptr;
}

bool HospitalSystem::isSlotAvailable(string doctorID, string dateTime)
{
    for (const auto &appt : appointments)
    {
        if (appt.doctorID == doctorID && appt.dateTime == dateTime &&
            (appt.status == "scheduled" || appt.status == "completed"))
        {
            return false;
        }
    }
    return true;
}

User *HospitalSystem::authenticateUser(string userID, string password)
{
    // Check doctors
    for (auto &doctor : doctors)
    {
        if (doctor.userID == userID && doctor.verifyPassword(password))
        {
            return &doctor;
        }
    }

    // Check patients
    for (auto &patient : patients)
    {
        if (patient.userID == userID && patient.verifyPassword(password))
        {
            return &patient;
        }
    }

    // Check admins
    for (auto &admin : admins)
    {
        if (admin.userID == userID && admin.verifyPassword(password))
        {
            return &admin;
        }
    }

    return nullptr;
}

Doctor *HospitalSystem::findDoctor(string doctorID)
{
    for (auto &doctor : doctors)
    {
        if (doctor.userID == doctorID)
        {
            return &doctor;
        }
    }
    return nullptr;
}

Patient *HospitalSystem::findPatient(string patientID)
{
    for (auto &patient : patients)
    {
        if (patient.userID == patientID)
        {
            return &patient;
        }
    }
    return nullptr;
}
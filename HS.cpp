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

// Appointment class definition
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

// HospitalSystem class definition
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

// Initialize static member
HospitalSystem *HospitalSystem::instance = nullptr;
HospitalSystem *g_HospitalSystemInstance = nullptr;

// Base User Class
class User
{
public:
    string userID;
    string name;
    string password; // Store hashed
    string role;     // "doctor", "patient", "admin"

    User() : role("user") {}
    User(string id, string n, string pwd, string r) : userID(id), name(n), password(hashPassword(pwd)), role(r) {}

    virtual ~User() {}
    virtual void displayMenu() = 0;
    bool verifyPassword(string inputPassword)
    {
        return hashPassword(inputPassword) == password;
    }
    void changePassword()
    {
        string oldPwd, newPwd;
        cout << "Enter current password: ";
        cin >> oldPwd;
        if (!verifyPassword(oldPwd))
        {
            cout << "Incorrect current password!" << endl;
            return;
        }
        cout << "Enter new password: ";
        cin >> newPwd;
        password = hashPassword(newPwd);
        cout << "Password changed successfully!" << endl;
        HospitalSystem::instance->logAudit("Password changed", userID);
    }
};

class Doctor : public User
{
public:
    string specialization;
    vector<string> availableSlots; // e.g., "2025-04-05 10:00"
    bool onEmergencyDuty = false;

    Doctor() : User() {}
    Doctor(string id, string n, string pwd, string spec) : User(id, n, pwd, "doctor"), specialization(spec) {}

    void viewAppointments();
    void markEmergency();
    void updateAvailability();
    void viewPatientHistory(string patientID);
    void displayMenu() override;
};

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

class Admin : public User
{
public:
    Admin() : User() {}
    Admin(string id, string n, string pwd) : User(id, n, pwd, "admin") {}

    void addDoctor();
    void addPatient();
    void generateReports();
    void manageEmergencyOverrides();
    void displayMenu() override;
};

// Implementation of Appointment methods
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

// Implementation of Doctor methods
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

// Implementation of Patient methods
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

// Implementation of Admin methods
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

// Implementation of HospitalSystem methods
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

# Hospital Scheduling System

## Overview

The Hospital Scheduling System is a comprehensive management solution designed to streamline healthcare operations through efficient scheduling and management of both medical professionals and patients. Built with modern C++ architecture, this system provides a robust platform for handling appointments, doctor availability, patient records, and administrative oversight with an emphasis on data persistence and system integrity.

## Features

### Core Functionality

- **Multi-User Role Management**: Distinct interfaces and capabilities for administrators, doctors, and patients
- **Appointment Management**: Comprehensive scheduling system with conflict detection and validation
- **Doctor Management**: Profile management, specialization tracking, and availability scheduling
- **Patient Records**: Secure patient information storage and retrieval
- **Audit Logging**: Complete system activity tracking for compliance and security
- **Data Persistence**: File-based storage ensuring data retention across sessions

### Administrative Capabilities

- User account management and oversight
- System-wide appointment monitoring
- Doctor and patient record administration
- Audit trail review and system analytics

### Doctor Features

- Personal schedule management
- Appointment viewing and confirmation
- Patient history access
- Availability configuration

### Patient Features

- Doctor search by specialization
- Appointment booking interface
- Schedule management
- Medical history viewing

## System Architecture

### Class Structure

```
User (Base Class)
├── Admin
├── Doctor
└── Patient

HospitalSystem (Core System)
└── Appointment (Scheduling Component)
```

### File Organization

- `User.h` - Abstract base class defining common user attributes
- `Admin.h/cpp` - Administrator functionality and system management
- `Doctor.h/cpp` - Doctor-specific operations and scheduling
- `Patient.h/cpp` - Patient management and appointment booking
- `Appointment.h/cpp` - Appointment handling and validation
- `HospitalSystem.h/cpp` - Core system orchestration and data management
- `HS1.cpp` - Main application entry point

### Data Files

- `doctors.txt` - Doctor profiles and credentials
- `patients.txt` - Patient records and information
- `appointments.txt` - Scheduled appointments database
- `audit_log.txt` - System activity and transaction log

## Installation

### Prerequisites

- C++ compiler with C++11 support or later (GCC, Clang, or MSVC)
- Standard Template Library (STL)
- File system access for data persistence

### Building the Project

#### Using GCC/G++

```bash
g++ -std=c++11 HS1.cpp HospitalSystem.cpp Admin.cpp Doctor.cpp Patient.cpp Appointment.cpp -o hospital_system
```

#### Using Visual Studio Code

1. Open the project folder in VS Code
2. Ensure C++ extension is installed
3. Use the provided `.vscode` configuration
4. Build using the integrated terminal or task runner

#### Running the Application

```bash
./hospital_system
```

On Windows:
```bash
HS1.exe
```

## Usage

### Getting Started

Upon launching the application, users are presented with a role-based login interface. Select your user type (Administrator, Doctor, or Patient) and provide the necessary credentials.

### Administrator Operations

1. **User Management**: Create, modify, or remove user accounts
2. **System Oversight**: Monitor all appointments and system activity
3. **Data Management**: Perform system-wide data operations
4. **Audit Review**: Access comprehensive system logs

### Doctor Operations

1. **Schedule Management**: Set availability and view appointments
2. **Patient Interaction**: Review patient information for scheduled appointments
3. **Profile Management**: Update specialization and contact information

### Patient Operations

1. **Book Appointments**: Search for available doctors and schedule visits
2. **Manage Schedule**: View, modify, or cancel existing appointments
3. **Medical History**: Access personal medical records

## Data Security

The system implements several security measures:

- Role-based access control ensuring users only access authorized features
- Audit logging of all system transactions
- Data validation to maintain integrity
- Secure file handling for persistent storage

## Technical Specifications

### Language and Standards

- **Language**: C++
- **Standard**: C++11 or higher
- **Paradigm**: Object-oriented programming with inheritance and polymorphism

### Dependencies

- Standard C++ Library
- File I/O operations (`<fstream>`)
- String manipulation (`<string>`)
- Container classes (`<vector>`, `<map>`)
- Date/time handling (`<ctime>`)

## Development Roadmap

### Planned Enhancements

- Database integration (MySQL/PostgreSQL) replacing file-based storage
- Web-based interface for remote access
- Automated appointment reminders via email/SMS
- Prescription management module
- Medical billing integration
- Real-time doctor availability updates
- Mobile application development
- Multi-language support

## Contributing

Contributions to the Hospital Scheduling System are welcome. Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/enhancement`)
3. Commit your changes with clear, descriptive messages
4. Push to your branch (`git push origin feature/enhancement`)
5. Submit a pull request with a comprehensive description

### Code Standards

- Follow C++ best practices and coding conventions
- Include appropriate comments and documentation
- Ensure all new features include error handling
- Test thoroughly before submitting pull requests

## License

This project is available for educational and non-commercial use.

## Contact

**Developer**: Purnika Khanal  

For bug reports, feature requests, or general inquiries, please open an issue on the GitHub repository.

## Acknowledgments

This project was developed as part of academic coursework to demonstrate object-oriented programming principles, file handling, and system design in healthcare management contexts.

---

**Note**: This system is intended for educational purposes and demonstration of programming concepts. For production healthcare environments, additional security measures, compliance certifications (HIPAA, GDPR), and professional medical software standards must be implemented.

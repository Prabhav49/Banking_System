# Banking Management System

## Overview
A secure, role-based banking management application utilizing Linux System Calls for robust process synchronization and data management.

## Features

### Roles and Functionalities

#### 1. Customer
- Secure login/logout
- Account balance viewing
- Money deposit/withdrawal
- Fund transfers
- Loan applications
- Transaction history
- Feedback submission
- Password changes

#### 2. Bank Employee
- Customer account management
- Loan application processing
- Transaction viewing
- Password changes

#### 3. Manager
- Customer account activation/deactivation
- Loan process assignment
- Customer feedback review
- Password changes

#### 4. Administrator
- Employee account management
- User role configuration
- Password changes

## Technical Architecture

### Core Technologies
- Socket Programming
- Linux System Calls
- Inter-Process Communication (IPC)
- File Management and Locking

### Key Technical Features
- Concurrent client connection handling
- Secure, role-based access control
- Data consistency maintenance
- Race condition prevention
- ACID property compliance

## Project Structure

### Source Files (`src/`)
- `addUser.c`: User registration
- `admin.c`: Administrator operations
- `client.c`: Client-side connection
- `customer.c`: Customer functionalities
- `employee.c`: Employee operations
- `login.c`: Authentication system
- `manager.c`: Manager functionalities
- `menu.c`: Role-specific menus
- `server.c`: Server-side request management

### Database Files (`db/`)
- `users.db`: User credentials
- `transactions.db`: Transaction records
- `loans.db`: Loan details
- `feed.db`: Customer feedback

### Header Files (`include/`)
- Role-specific header files for function declarations
- Shared constants and definitions

## Setup and Installation

### Prerequisites
- GCC Compiler
- Linux/Unix Environment
- pthread Library

### Installation Steps
1. Clone the repository
   ```bash
   git clone https://github.com/Prabhav49/Banking_System.git
   cd BankingManagementSystem
   ```

2. Compile the project
   ```bash
   cd src
   gcc -o server server.c -lpthread
   gcc -o client client.c -lpthread
   ```

3. Run the server
   ```bash
   ./server
   ```

4. Run the client
   ```bash
   ./client
   ```

## Synchronization Mechanisms
- File locking
- Mutex-based resource protection
- Single-session user management
- Concurrent access handling

## Security Features
- Role-based access control
- Secure authentication
- Transaction isolation
- Data integrity protection

## Contributing
1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License
© 2024 Prabhav Pandey, IIITB. All Rights Reserved.

## Contact
For issues or inquiries, please open a GitHub issue or
Prabhav Pandey - Prabhav.Pandey@iiitb.ac.in

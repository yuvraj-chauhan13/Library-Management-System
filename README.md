# 📚 Library Management System

<p align="center">

![C](https://img.shields.io/badge/C-Language-blue?style=for-the-badge&logo=c)
![File Handling](https://img.shields.io/badge/File%20Handling-Persistent-success?style=for-the-badge)
![Makefile](https://img.shields.io/badge/Build-Makefile-orange?style=for-the-badge)
![CLI](https://img.shields.io/badge/Application-Terminal-black?style=for-the-badge)
![Course Project](https://img.shields.io/badge/CSP-Course%20Project-brightgreen?style=for-the-badge)

</p>

A **terminal-based Library Management System** developed in **C** using **modular programming**, **file handling**, and a **role-based authentication system**. The application provides efficient management of books, users, loans, and borrowing activities while maintaining persistent data using text files.

---

# ✨ Features

### 👤 User Authentication
- Secure login system
- User registration
- Role-based access control

### 👥 User Roles

- **Student**
  - Search books
  - Borrow books
  - Return books
  - View borrowed books

- **Librarian**
  - Add books
  - Remove books
  - View complete book catalog

- **Administrator**
  - View all registered users
  - View loan history
  - Monitor system records

---

# 📖 Book Management

- Add new books
- Remove existing books
- Search books by keyword
- List all available books
- Track available copies
- Automatically generate Book IDs

---

# 📚 Borrow & Return System

- Borrow books
- Return books
- Automatic availability update
- Loan record generation
- Due date generation
- Late fine calculation
- Loan history management

---

# 💾 Persistent Storage

The system stores all information permanently using text files.

```
data/
│
├── books.txt
├── users.txt
└── loans.txt
```

No external database is required.

---

# 🏗️ Project Structure

```
Library-Management-System/
│
├── books.c
├── books.h
├── users.c
├── users.h
├── storage.c
├── storage.h
├── menu.c
├── menu.h
├── main.c
├── Makefile
│
├── data/
│   ├── books.txt
│   ├── users.txt
│   └── loans.txt
│
├── README.md
└── .gitignore
```

---

# 🧩 Modules

| Module | Description |
|---------|-------------|
| **books.c** | Book searching, adding, removing, borrowing and returning |
| **users.c** | User registration and authentication |
| **storage.c** | File handling and persistent storage |
| **menu.c** | Interactive terminal menus |
| **main.c** | Application entry point |
| **Makefile** | Project compilation |

---

# ⚙️ Technologies Used

- C Programming
- Modular Programming
- File Handling
- Dynamic Memory Allocation
- Makefile
- Command Line Interface (CLI)

---

# 🚀 Build & Run

## Compile

```bash
make
```

## Execute

```bash
./library
```

---

# 📋 Application Flow

```
Start Application
        │
        ▼
 Login / Register
        │
        ▼
Authenticate User
        │
        ▼
 ┌───────────────┐
 │ Student       │
 │ Librarian     │
 │ Administrator │
 └───────────────┘
        │
        ▼
 Perform Operations
        │
        ▼
Update Files Automatically

---

# 🔑 Key Features

- Modular source code
- Role-based authentication
- Persistent file storage
- Borrow and return tracking
- Automatic Book ID generation
- Automatic Loan ID generation
- Due date management
- Fine calculation
- Dynamic file updates
- Command-line interface
- Makefile support

---

# 📈 Future Improvements

- Password hashing
- Search by Author/ISBN
- Book reservation system
- Transaction logs
- CSV export
- SQLite/MySQL integration
- Colored terminal interface
- Unit testing
- GUI version

---

# 🎯 Learning Outcomes

This project demonstrates practical implementation of

- Modular Programming
- File Handling
- Dynamic Memory Management
- Structures
- String Manipulation
- Authentication System
- Role-Based Access Control
- Data Persistence
- Software Design
- Makefile Build System

---

# 👨‍💻 Author

**Yuvraj Chauhan**

- GitHub: https://github.com/yuvraj-chauhan13

---

# ⭐ Support

If you found this project useful, consider giving it a ⭐ on GitHub.
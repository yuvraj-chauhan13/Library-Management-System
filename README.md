# 📚 Advanced Library Management System

<p align="center">

![C](https://img.shields.io/badge/C-Language-blue?style=for-the-badge&logo=c)
![Makefile](https://img.shields.io/badge/Build-Makefile-orange?style=for-the-badge)
![CLI](https://img.shields.io/badge/Application-Terminal-black?style=for-the-badge)
![File Handling](https://img.shields.io/badge/File%20Handling-Persistent-success?style=for-the-badge)

</p>

A modular **Library Management System** built in **C** that demonstrates file handling, modular programming, authentication, and role-based access control. The system manages books, users, loans, fines, and activity logs while storing all data persistently in text files.

---

# ✨ Features

### 👤 Authentication
- User Registration
- Secure Login
- Password Hashing
- Password Validation
- Role-Based Access Control

### 📚 Book Management
- Add & Remove Books
- Search Books
- List Available Books
- Automatic Book ID Generation
- Book Popularity Tracking

### 📖 Loan Management
- Borrow Books
- Return Books
- Due Date Management
- Automatic Fine Calculation
- Personal Loan History
- Complete Loan Records

### 📊 Reports
- Library Statistics
- Top Borrowed Books
- Overdue Books Report

### 📝 Activity Logs
- Registration Logs
- Login & Failed Login Logs
- Borrow / Return Logs
- Book Addition & Removal Logs

---

# 👥 User Roles

### 🎓 Student
- Search Books
- Borrow Books
- Return Books
- View Personal Loans

### 📚 Librarian
- Add Books
- Remove Books
- View Book Catalog

### 👨‍💼 Administrator
- View Users
- View Loans
- View Activity Logs
- View Library Statistics
- View Top Borrowed Books
- View Overdue Books Report

---

# 🗂 Project Structure

```text
Library-Management-System/
│
├── main.c
├── menu.c
├── users.c
├── books.c
├── loans.c
├── logs.c
├── statistics.c
├── security.c
│
├── *.h
├── Makefile
│
├── data/
│   ├── users.txt
│   ├── books.txt
│   ├── loans.txt
│   └── logs.txt
│
├── README.md
└── .gitignore
```

---

# ⚙️ Technologies Used

- C Programming
- Modular Programming
- File Handling
- Dynamic Memory Allocation
- Makefile
- Command-Line Interface (CLI)

---

# 🚀 Build & Run

### Compile

```bash
make
```

### Run

```bash
./library
```

---

# 📈 Sample Workflow

```text
Start
   │
   ▼
Login / Register
   │
   ▼
Authenticate User
   │
   ▼
Student / Librarian / Admin
   │
   ▼
Perform Operations
   │
   ▼
Update Files Automatically
```

---

# 💾 Persistent Storage

The application stores all data using plain text files.

| File | Purpose |
|------|---------|
| `users.txt` | User Accounts |
| `books.txt` | Book Records |
| `loans.txt` | Loan Records |
| `logs.txt` | Activity Logs |

No external database is required.

---

# 🎯 Learning Outcomes

This project demonstrates practical experience with:

- Modular Software Design
- File Handling in C
- Structures & Dynamic Memory
- Authentication Systems
- Password Hashing
- Role-Based Access Control
- Data Persistence
- Makefile-Based Build Systems

---

# 🔮 Future Improvements

- Book Reservation System
- Search by Author / ISBN
- SQLite / MySQL Integration
- Email Notifications
- Graphical User Interface (GUI)

---

# 👨‍💻 Author

**Yuvraj Chauhan**

- GitHub: **https://github.com/yuvraj-chauhan13**
- LinkedIn: *(Add your LinkedIn profile here)*

---

If you found this project useful, consider giving it a ⭐ on GitHub.
#  Restaurant Billing System (C++ + Web)

##  Overview

This project presents a Restaurant Billing System implemented in two different ways:

1. **C++ Console Application** – A traditional file-based system
2. **Web Application** – A modern browser-based interface using HTML, CSS, and JavaScript

Both modules are independent and demonstrate different approaches to solving the same real-world problem.

---

##  Objectives

* Automate restaurant billing process
* Reduce manual calculation errors
* Demonstrate both traditional and modern development approaches
* Provide a simple and efficient user experience

---

##  Features

###  C++ Console Version

* Menu management (add, modify, delete items)
* Customer detail entry
* Order placement system
* Invoice generation with GST & service tax
* File handling for data storage

---

###  Web Version

* Interactive user interface
* Admin login system
* Customer ordering system
* Cart and billing interface
* Data stored using browser localStorage

---

##  Technologies Used

* **C++** (File Handling, Console UI)
* **HTML, CSS, JavaScript** (Frontend Web Development)

---

##  How to Run

###  C++ Version

1. Compile:

```bash
g++ rest.cpp -o app
```

2. Run:

```bash
./app
```

---

###  Web Version

1. Open `index.html` in any browser
2. Use login:

   * Admin → `admin / 2004`
   * Customer → Enter name & mobile

---

##  Project Structure

```
restaurant-billing-system/
│
├── cpp-version/
│   └── rest.cpp
│
├── web-version/
│   ├── index.html
│   ├── style.css
│   └── app.js
│
└── README.md
```
---

##  Limitations

* No database (uses files and localStorage)
* No real integration between C++ and Web modules
* Basic authentication system
* Console version is platform-dependent (uses conio.h)

---

##  Future Scope

* Integration using backend (Node.js / API)
* Database support (MySQL / MongoDB)
* Online payment system
* Enhanced UI/UX design

---

##  Author

**Ahana Gupta**

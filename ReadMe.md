# Taxi Management System (CS-201 Project)

A C++ application that simulates a taxi routing and management system using **van Emde Boas Trees** and **Dijkstra's algorithm** to find optimal routes and nearby drivers.

---

## Features

- **Interactive GUI** for location input
- **Real-time route calculation and visualization**
- **Nearby driver detection**
- **Fare estimation**
- **Visual representation** of pickup points, destinations, available taxis and shortest route

---

## Requirements

- C++ Compiler with C++11 support
- wxWidgets 3.0 or later

---

## Quick Start

### Install wxWidgets


#### Ubuntu/Debian
```bash
sudo apt-get install libwxgtk3.0-dev
```

#### macOS
```bash
brew install wxwidgets
```

### Windows
For installing on windows, visit https://www.wxwidgets.org/downloads/


---
## Build and Run
Navigate to the project folder and use the following commands

For compiling GUI Application:
```bash
g++ -std=c++11 final_code_GUI.cpp `wx-config --cxxflags --libs` -o final_code_GUI
```

For running GUI Application:
```bash
./final_code_GUI
```

For compiling Console Application:
```bash
g++ -std=c++11 final_code_console.cpp -o final_code_console
```

For running GUI Application:
```bash
./final_code_console
```
---
##Usage
1. Run the application
2. Enter pickup and destination coordinates 
3. Click "Find Drivers" to view:
    - Available drivers
    - Shortest route
    - Fare estimate and ETA

---

## Contributors

- **Gitansh Bansal** (2023MCB1294)
- **Mohakjot Dhiman** (2023MCB1302)
- **Kanav Puri** (2023MCB1298)

---

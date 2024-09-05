# Digital Scope: Software for Visualization of IEC 61850 Sampled Values (Linux Only)

## Overview

This project was implemented as an Industrial Immersion project in cooperation between [Tekvel](https://tekvel.com) and [Skoltech](https://www.skoltech.ru). It provides a graphical tool for visualizing IEC 61850 Sampled Values, enabling users to interact with and analyze data streams from various sources. The application is capable of working with SV streams that have a basic structure.

## Prerequisites

- **CMake** (version 3.22 or higher)
- **C++ compiler** (GCC, Clang, or MSVC)
- **WxWidgets library** (current stable version 3.2.x)

## Building on Linux (Ubuntu)

### Installing the Prerequisites
```bash
sudo apt install cmake make git
```

### Building the Project
```bash
git clone https://github.com/tekvel/scopex.git
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cd ..
cmake --build build
```

> **Note:** If wxWidgets has not been installed in the system, specify the path to the wxWidgets build directory when configuring the project for building:
```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/wxWidgets_lib/wxWidgets-3.2.x ..
```

### Running the Application
```bash
sudo ./build/prog
```

## Screenshots

- Network Selection Dialog:

![image 1](https://github.com/user-attachments/assets/0bcba107-a3e1-48c4-9849-6ab41e8592c5)

- SV Selection Dialog:

![image 2](https://github.com/user-attachments/assets/feb3a685-3c59-4aa6-8d27-1c31b6faa0bc)

- App view with two graphical panels:

![image 3](https://github.com/user-attachments/assets/4566b73d-5e10-4f7b-a144-9d63e82f04af)

## Roadmap

- [x] Graphical panels for drawing signals
- [x] Network Interface Selection dialog
- [x] SV Parser
- [x] SV Selection dialog
- [x] Interface adjustment depending on the number of measurements in the SV structure
- [x] Toolbar: Start and Stop buttons and combobox with SV streams
- [x] Simple Signal Analyzer for signal magnitude determination

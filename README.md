# Digital Scope: Software for Visualization of IEC 61850 Sampled Values

## Prerequisites

- CMake (version 3.22 or higher)
- A C++ compiler (GCC, Clang, or MSVC)
- WxWidgets library (current stable version 3.2.x)

### Building on Linux (Ubuntu)
Installing the Prerequisites:
```bash
sudo apt install cmake make git
```
Building:
```bash
git clone https://github.com/tekvel/scopex.git
mkdir build
cd build
cmake ..
cd ..
cmake --build build
```
If wxWidgets has not been installed in the system specify path to wxWidgets build directory when configuring the project for building:
```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/wxWigets_lib/wxWidgets-3.2.x ..
```
Running:
```bash
./build/prog
```

---

## This project is currently in the development stage
### Roadmap

- [x] Graphical panels for drawing signals
- [ ] The ability to select a network interface
- [ ] The ability to subscribe to various SV streams
- [ ] SV parser
- [ ] The ability to determine signal parameters
- [ ] The ability to upload/download signals
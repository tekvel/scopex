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
Running:
```bash
./build/prog
```

cmake_minimum_required(VERSION 3.22.1)

project(prog VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(wxWidgets REQUIRED COMPONENTS core base)

include(${wxWidgets_USE_FILE})

add_executable(prog
    src/main.cpp
    src/communicate.cpp
    src/panels.cpp
    src/sinusoidal_panel.cpp
    src/signals.cpp
)

target_link_libraries(prog ${wxWidgets_LIBRARIES})

target_include_directories(prog PRIVATE ${wxWidgets_INCLUDE_DIRS} include)

set(HEADER_FILES
    include/main.h
    include/communicate.h
    include/panels.h
    include/sinusoidal_panel.h
    include/signals.h
)

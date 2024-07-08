cmake_minimum_required(VERSION 3.22.1)

project(prog VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the root directory of your wxWidgets installation
# set(wxWidgets_ROOT_DIR "/home/nkb/Documents/wxWigets_lib/wxWidgets-3.2.5")

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Find pcap using pcap-config
find_package(PkgConfig REQUIRED)
pkg_search_module(PCAP REQUIRED libpcap)
include_directories(${PCAP_INCLUDE_DIRS})

# Collect all source files
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Collect all header files
file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# Add executable target
add_executable(prog ${SOURCE_FILES} ${HEADER_FILES})

# Link wxWidgets libraries
target_link_libraries(prog ${wxWidgets_LIBRARIES})
target_link_libraries(prog ${PCAP_LIBRARIES})

# Include directories
target_include_directories(prog PRIVATE ${wxWidgets_INCLUDE_DIRS} include)

# Include specific build type settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

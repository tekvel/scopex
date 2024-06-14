cmake_minimum_required(VERSION 3.22.1)

project(prog VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the root directory of your wxWidgets installation
# set(wxWidgets_ROOT_DIR "/home/nkb/Documents/wxWigets_lib/wxWidgets-3.2.5")

find_package(wxWidgets REQUIRED COMPONENTS core base)

include(${wxWidgets_USE_FILE})

# Collect all source files
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Collect all header files
file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

# Add executable target
add_executable(prog ${SOURCE_FILES} ${HEADER_FILES})

# Link wxWidgets libraries
target_link_libraries(prog ${wxWidgets_LIBRARIES})

# Include directories
target_include_directories(prog PRIVATE ${wxWidgets_INCLUDE_DIRS} include)


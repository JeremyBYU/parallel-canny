cmake_minimum_required(VERSION 3.14)
# Project Details
set(PROJECT_NAME "PCanny")
set(PROJECT_VERSION 0.0.1)
# Set Project Properties
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION}
DESCRIPTION "C++ Canny Edge Detector Example"
LANGUAGES CXX)
# Set Global Properties
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release)
endif()

# Include cmake folder
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Extras.cmake)

# Output Folders
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Options for SPL
option(PCAN_BUILD_PYMODULE "SPL -Build Python Module" ON)
option(PCAN_BUILD_EXAMPLES "SPL - Build Examples" ON)
option(PCAN_WITH_OPENMP "SPL - Build with OpenMP Support" ON)
option(PCAN_BUILD_BENCHMARKS "SPL - Build Benchmarks" ON)
option(PCAN_BUILD_TESTS "SPL - Build Tests" OFF)

# Add any dependencies needed by our library
add_subdirectory("thirdparty")

# Build our library
add_subdirectory("src")

# Build examples if configured
if(PCAN_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

# Build tests if configured
if(PCAN_BUILD_TESTS)
    add_subdirectory("tests")
endif()

# Build benchmarks if configured
if(PCAN_BUILD_BENCHMARKS)
    add_subdirectory("bench")
endif()



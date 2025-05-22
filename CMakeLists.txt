# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.15)

# Define the project name, language, version, and description
project(BMP_Sketcher
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "BMP Image Processing"
)

# Set C++ standard to C++17 and enforce it without compiler extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add strict compiler warnings depending on the compiler used
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif()

# Project build options to enable or disable optional components
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" ON)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

# Find required external packages
find_package(OpenMP REQUIRED)
find_package(OpenCL REQUIRED)
find_package(Threads REQUIRED)

# Collect all source and header files from src and include directories
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.hpp"
    "include/*.hpp"
)

# Exclude main.cpp from the library sources
list(FILTER SOURCES EXCLUDE REGEX ".*main.cpp$")
set(MAIN_SOURCE "src/main.cpp")

# Create a static library for the core functionality
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})

# Specify include directories for the library (public and private)
target_include_directories(${PROJECT_NAME}_lib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link core library with required system libraries
target_link_libraries(${PROJECT_NAME}_lib
    PUBLIC
        OpenMP::OpenMP_CXX
        OpenCL::OpenCL
        Threads::Threads
)

# Define the main executable and link it with the core library
add_executable(${PROJECT_NAME} ${MAIN_SOURCE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

# Installation rules for executables, libraries, and headers
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_lib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

# Unit test configuration
if(BUILD_TESTS)
    enable_testing()

    # Find GoogleTest (must be installed or provided externally)
    find_package(GTest REQUIRED)

    # Create test executable
    add_executable(${PROJECT_NAME}_tests
        tests/test_example.cpp
        # Add other test files here
    )

    # Link test executable with project library and GTest
    target_link_libraries(${PROJECT_NAME}_tests
        PRIVATE
            ${PROJECT_NAME}_lib
            GTest::GTest
            GTest::Main
    )

    # Register the test with CTest
    add_test(NAME ${PROJECT_NAME}_unit_tests
        COMMAND ${PROJECT_NAME}_tests
    )

    # Enable code coverage (only supported with GCC or Clang)
    if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME}_tests PRIVATE --coverage -O0 -g)
        target_link_options(${PROJECT_NAME}_tests PRIVATE --coverage)
    endif()
endif()

# Benchmark configuration
if(BUILD_BENCHMARKS)
    # Find Google Benchmark (must be installed or provided externally)
    find_package(benchmark REQUIRED)

    # Create benchmark executable
    add_executable(${PROJECT_NAME}_benchmarks
        benchmarks/main.cpp
        # Add other benchmark files here
    )

    # Link benchmark executable with project library and benchmark library
    target_link_libraries(${PROJECT_NAME}_benchmarks
        PRIVATE
            ${PROJECT_NAME}_lib
            benchmark::benchmark
    )
endif()

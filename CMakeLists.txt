cmake_minimum_required(VERSION 3.15)

# Set the toolchain file path before the project command
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/Debug/generators/conan_toolchain.cmake)

project(WebsiteProject)

set(CMAKE_CXX_STANDARD 17)

# Find the required packages
find_package(httplib CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

# Automatically add all .cpp files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(WebServer ${SOURCES})

# Include directories
target_include_directories(WebServer PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link libraries
target_link_libraries(WebServer PRIVATE 
    httplib::httplib 
    OpenSSL::SSL 
    OpenSSL::Crypto
    JsonCpp::JsonCpp
)

# Copy HTML files to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/app DESTINATION ${CMAKE_BINARY_DIR})

# Debug configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# Release configuration
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

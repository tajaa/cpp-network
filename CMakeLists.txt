cmake_minimum_required(VERSION 3.15)

# Set the toolchain file path as a cache variable
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/Debug/generators/conan_toolchain.cmake CACHE PATH "Path to the Conan generated toolchain file")

# Include the Conan-generated toolchain file
include(${CMAKE_TOOLCHAIN_FILE})

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

# Copy config file to the build directory
configure_file(${CMAKE_SOURCE_DIR}/config.jsonc ${CMAKE_BINARY_DIR}/config.jsonc COPYONLY)

# Copy SSL certificate and key to the build directory
configure_file(${CMAKE_SOURCE_DIR}/localhost.crt ${CMAKE_BINARY_DIR}/localhost.crt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/localhost.key ${CMAKE_BINARY_DIR}/localhost.key COPYONLY)

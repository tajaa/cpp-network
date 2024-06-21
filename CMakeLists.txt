cmake_minimum_required(VERSION 3.15)
project(cpp_web_project)

set(CMAKE_CXX_STANDARD 17)

# Set the path to the Conan-generated files
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/Debug/generators ${CMAKE_PREFIX_PATH})

# Find and include the Conan-generated configuration
find_package(httplib CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib OpenSSL::SSL OpenSSL::Crypto)

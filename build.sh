#!/bin/bash

# Function to perform a clean build
clean_build() {
    echo "Performing a clean build..."
    rm -rf build
    mkdir -p build
    cd build
    if ! conan install .. --build=missing -s build_type=Debug -pr:b=default -pr:h=default; then
        echo "Conan install failed"
        exit 1
    fi
    
    # Print Conan info
    echo "Conan installation completed. Printing info:"
    conan info ..
    
    # Find inja.hpp
    echo "Searching for inja.hpp:"
    find . -name "inja.hpp"
    
    if ! cmake .. -DCMAKE_TOOLCHAIN_FILE=Debug/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug; then
        echo "CMake configuration failed"
        exit 1
    fi
    if ! cmake --build . --config Debug; then
        echo "CMake build failed"
        exit 1
    fi
    cd ..
}

# Check if the user wants a clean build
if [ "$1" == "clean" ]; then
    clean_build
else
    # Check if build directory exists, if not, do initial setup
    if [ ! -d "build" ]; then
        clean_build
    else
        # Build the project
        cd build
        if ! cmake --build . --config Debug; then
            echo "CMake build failed"
            exit 1
        fi
        cd ..
    fi
fi

# Copy config file to build directory
cp config.jsonc build/Debug/

# Check if SSL files exist and copy them
if [ -f "localhost.crt" ] && [ -f "localhost.key" ]; then
    cp localhost.crt build/Debug/
    cp localhost.key build/Debug/
else
    echo "Warning: SSL certificate files not found. HTTPS might not work."
fi

#verify the SSL files
echo "Checking SSL files..."
ls -l localhost.crt localhost.key 
echo "Contents of build/Debug directory"
ls -l build/Debug 

# Find and run the executable
EXECUTABLE=$(find build -name WebServer -type f -perm +111)
if [ -n "$EXECUTABLE" ]; then
    echo "Running $EXECUTABLE"
    cd $(dirname "$EXECUTABLE")  # Change to the directory containing the executable
    ./WebServer
else
    echo "Error: WebServer executable not found"
fi

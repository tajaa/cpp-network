#!/bin/bash

# Function to perform a clean build
clean_build() {
    echo "Performing a clean build..."
    rm -rf build
    mkdir -p build
    cd build
    conan install .. --build=missing -s build_type=Debug
    cmake .. -DCMAKE_TOOLCHAIN_FILE=./Debug/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
    cmake --build . --config Debug
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
        cmake --build . --config Debug
        cd ..
    fi
fi

# Copy config and SSL files to build directory
cp config.jsonc build/
cp localhost.crt build/
cp localhost.key build/

# Find and run the executable
EXECUTABLE=$(find build -name WebServer -type f)
if [ -n "$EXECUTABLE" ]; then
    echo "Running $EXECUTABLE"
    cd build  # Change to build directory before running
    ./WebServer
else
    echo "Error: WebServer executable not found"
fi

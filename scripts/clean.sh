#!/bin/bash

# This script cleans up the source tree leaving it as if a fresh clone of
# the repository was made.

# Source the project configuration.
source config.sh

# Remove the binary directory.
if [ -d $AOC_BIN_DIR ]
then
    echo -e "${LGREEN}Removing '$AOC_BIN_DIR'${NC}"
    rm -r $AOC_BIN_DIR
fi

# Remove the CMake build directory.
if [ -d $AOC_BUILD_DIR ]
then
    echo -e "${LGREEN}Removing '$AOC_BUILD_DIR'${NC}"
    rm -r $AOC_BUILD_DIR
fi

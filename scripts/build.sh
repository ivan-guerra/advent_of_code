#!/bin/bash

# This script builds all AOC solutions. Run 'build.sh -h' to see all the
# script options.

BUILD_TYPE="RELEASE"

source config.sh

Help()
{
    echo "Build AOC Solutions"
    echo
    echo "usage: build.sh [OPTION]..."
    echo "options:"
    echo -e "\tg    Enable debug info."
    echo -e "\th    Print this help message."
}

Main()
{
    # Create the build directory if it does not already exist.
    mkdir -pv $AOC_BUILD_DIR

    pushd $AOC_BUILD_DIR
    cmake ../                               \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE && \
        make -j$(nproc) all                  && \
        make install

    # Exit if any of the above commands fails.
    if [ $? -ne 0 ];
    then
        exit 1
    fi
    popd
}

while getopts ":hg" flag
do
    case "$flag" in
        g) BUILD_TYPE="DEBUG" ;;
        h) Help
            exit ;;
        \?) echo "error: invalid option '$OPTARG'"
            Help
            exit ;;
    esac
done

Main

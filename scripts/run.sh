#!/bin/bash

# This script allows the User to run a solution's corresponding executable.
# The script usage is as follows: run.sh SOLUTION_NAME INPUT_FILE
# where SOLUTION_NAME is an executable name of the form aoc_[1-25]_[1,2]
# For example, run.sh aoc_5_2 tells the script to run the day 5 problem 2
# solution.

source config.sh

if [ $# -eq 0 ]
then
    echo -e "${LRED}error missing executable name${NC}"
    echo -e "${LRED}usage: run.sh SOLUTION_NAME INPUT_FILE${NC}"
    echo -e "${LRED}EXAMPLE\n\trun.sh day_1_1 calories.txt${NC}"
    exit 1
fi

EXE_NAME="$1"
INPUT_FILE="$2"

# Parse the day number out of the executable name. The day number
# is always the first decimal number between the '_' characters in the executable
# name. For example, given the name 'aoc_25_1', the statement below will set
# DAY_NUM=25
DAY_NUM=$(echo ${EXE_NAME} | sed -n "s/^.*_\([0-9]\+\)_.*$/\1/p")

EXE_PATH="${AOC_BIN_DIR}/day_${DAY_NUM}/${EXE_NAME}"
if [ -f "${EXE_PATH}" ]
then
    # The executable was found, run it.
    ${EXE_PATH} ${INPUT_FILE}
else
    echo -e "${LRED}error executable '${EXE_PATH}' not found${NC}"
    echo -e "${LRED}make sure you run build.sh before running this script${NC}"
fi

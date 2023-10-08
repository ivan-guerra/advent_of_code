#!/bin/bash

# This script allows the User to run a solution's corresponding executable.
# The script usage is as follows: run.sh [OPTION]... EXE_NAME INPUT_FILE
# where EXE_NAME is the name of the solution executable in the format
# aoc_YYYY_[1-25]_[1-2] and INPUT_FILE is the path to the problem input file.
# For example, ./run.sh aoc_2022_5_2 inputs/2022/day_05/stacks.txt
# tells the script to run the AOC year 2022 day 5 problem 2 solution taking
# as input the text file located under inputs/2022/day_05/stacks.txt.

source config.sh

DEBUG_ON=0

Help()
{
    echo "Run AOC Solutions"
    echo
    echo "usage: run.sh [OPTION]... EXE_NAME INPUT_FILE"
    echo "options:"
    echo -e "\tg    Load the executable into the GNU Debugger."
    echo -e "\th    Print this help message."
}

RunSolution()
{
    EXE_PATH=$(find $AOC_BIN_DIR -name $1 2>/dev/null)
    if [ -z $EXE_PATH ]; then
        echo -e "${LRED}error: executable '$1' not found${NC}"
        echo -e "${LRED}Did you forget to run build.sh?${NC}"
        exit 1
    fi

    INPUT_FILE=$2
    if [ ! -f $INPUT_FILE ]; then
        echo -e "${LRED}error: file '$INPUT_FILE' does not exist${NC}"
        exit 1
    fi

    if [ $DEBUG_ON -eq 1 ];
    then
        gdb --args $EXE_PATH $INPUT_FILE
    else
        $EXE_PATH $INPUT_FILE
    fi
}

Main()
{
    if [ $# -ne 2 ]
    then
        echo -e "${LRED}error: invalid argument count${NC}"
        Help
        exit 1
    fi
    RunSolution $1 $2
}

while getopts "gh" flag;
do
    case "$flag" in
        g) DEBUG_ON=1 ;;
        h) Help
            exit ;;
        \?) echo "error: invalid option '$OPTARG'"
            Help
            exit ;;
    esac
done
shift $((OPTIND - 1))

Main $@

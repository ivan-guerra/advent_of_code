# Advent of Code

Advent of Code solutions in C++.

### Building the Solutions

To build this project you'll need CMake3.13+ and a C++ compiler supporting
C++20 features.

The [scripts](scripts) directory contains a few utility scripts including the
build and run scripts. Change directory to [scripts](scripts).
```
cd scripts/
```

To build all solutions:
```
./build.sh
```

You can build the solutions with debug info by passing the build script the
`-g` flag:
```
./build.sh -g
```
This is useful if you intend to debug a solution with GDB or some other
debugging tool. See the [Running the Solutions](#running-the-solutions) section
for details on how to run under GDB.

### Running the Solutions

Solutions can be run using the `run.sh` script included in the `scripts`
directory. `run.sh` takes two positional arguments: the name of the
solution executable to be run and the path to the solution's input file.
All executables have the form `aoc_YYYY_[1-25]_[1-2]`. For example, to
run AOC 2022's day 5 exercise 2 solution you would use the command
```
./run.sh aoc_2022_5_2 ../inputs/2022/day_05/stacks.txt
```

If you built the solutions with debug info, you can tell `run.sh` to
load a particular solution under the GNU Debugger by passing the `-g` flag.
As an example, suppose we wanted to debug the AOC 2022 day 5 exercise 2
solution, then we would run the command:
```
./run.sh -g aoc_2022_5_2 inputs/2022/day_05/stacks.txt
```

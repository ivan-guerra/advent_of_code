# Advent of Code

Advent of Code solutions in C++.

### Building the Solutions

To build this project you'll need CMake3.28+ and a C++ compiler supporting
C++23 features.

To configure the project:

```bash
cmake --preset [release|debug]
```

To build and install all solutions:

```bash
cmake --build --preset [release|debug]
```

### Running the Solutions

Each solution is a standalone binary installed under `bin/`. All binaries follow
the naming convention `aoc_${YEAR}_${DAY}_${SOLN}`. All solution binaries have a
single required argument that is the `input.txt` with the puzzle input. You can
find all input text files under `inputs/`.

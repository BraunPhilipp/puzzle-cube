## Puzzle Cube

**Summary**

This project's goal is to solve puzzle cube with the side length 4. _cube.py_ and _random.py_ are prototypes for the presented C++ implementation. _cube.py_ searches the densest possible combination in each iteration. The algorithm performed ~20x better in testing than randomly placing parts into the cube.

**Execution**

_cube.cpp_ is written in C++11 to compile simply use `g++ -std=C++0x -o cube cube.cpp`. To ensure constant uptime or to open multiple processes on an Ubuntu machine `nohup sh spawn.sh &`. To close the processes `ps -ef` and `kill -9 PID`.

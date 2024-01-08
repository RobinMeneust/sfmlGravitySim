# SFML Gravity Simulator

## About

Small project to learn how to use SFML.
Simulate particules collision and use continuous collision.
Some bugs (crashes) are still not fixed in this version.

## Install dependencies
`cmake -S . -B build`

## Build and run
`cmake --build .\build`

## Debug

- `cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug`
- `cmake --build build-debug`
- `gdb build-debug/simple_example`

## Help

[PDF](collision_detection.pdf)

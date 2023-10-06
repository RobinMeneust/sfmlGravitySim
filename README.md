## Install dependencies
`cmake -S . -B build`

## Build and run
`cmake --build .\build`

## Debug

- `cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug`
- `cmake --build build-debug`
- `gdb build-debug/simple_example`
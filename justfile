# Set up the project and build directory
setup generator="Ninja":
    cmake -S . -B build -G{{generator}}

# Compile the project
build:
    cmake --build build

# Run all available tests
test: build
    ctest --test-dir build

# Clean the build directory
clean:
    cmake --build build -t clean

# Check the project using Cppcheck
check:
    cppcheck --project=build/compile_commands.json -ibuild --check-level=exhaustive

# taskStubs
PerfStubs...but for Tasks

Most inspiration from https://github.com/UO-OACISS/perfstubs

Some inspiration from https://github.com/ashchetson/SimplePlugin

This task wrapper API is intended to be used as a generic tool interface for wrapping asynchronous tasks in a multithreaded, heterogeneous tasking runtime.

## To build:

```
module load cmake
cmake -B build
cmake --build build --parallel
ctest --test-dir build --verbose
```

...which should look like:

```
[khuck@voltar taskStubs]$ cmake -B build && cmake --build build --parallel
-- The CXX compiler identification is GNU 8.5.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.2s)
-- Generating done (0.0s)
-- Build files have been written to: /home/users/khuck/src/taskStubs/build
[ 16%] Building CXX object TimerPlugin/CMakeFiles/TimerPlugin.dir/Plugin.cpp.o
[ 33%] Building CXX object ToolExample/CMakeFiles/MyTool.dir/MyTool.cpp.o
[ 50%] Linking CXX static library libTimerPlugin.a
[ 50%] Built target TimerPlugin
[ 66%] Building CXX object CMakeFiles/main.dir/main.cpp.o
[ 83%] Linking CXX shared library libMyTool.so
[ 83%] Built target MyTool
[100%] Linking CXX executable main
[100%] Built target main
[khuck@voltar taskStubs]$ ctest --test-dir build --verbose
Internal ctest changing into directory: /home/users/khuck/src/taskStubs/build
UpdateCTestConfiguration  from :/home/users/khuck/src/taskStubs/build/DartConfiguration.tcl
UpdateCTestConfiguration  from :/home/users/khuck/src/taskStubs/build/DartConfiguration.tcl
Test project /home/users/khuck/src/taskStubs/build
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
test 1
    Start 1: testWithout

1: Test command: /home/users/khuck/src/taskStubs/build/main
1: Working Directory: /home/users/khuck/src/taskStubs/build
1: Test timeout computed to be: 10000000
1/2 Test #1: testWithout ......................   Passed    0.00 sec
test 2
    Start 2: testWith

2: Test command: /home/users/khuck/src/taskStubs/build/main
2: Working Directory: /home/users/khuck/src/taskStubs/build
2: Environment variables: 
2:  TASKTIMER_EXTERNAL_TOOL_LIBRARY=/home/users/khuck/src/taskStubs/build/ToolExample/libMyTool.so
2: Test timeout computed to be: 10000000
2: Created timer main with ID 0 and parents []
2: Scheduling timer main with ID 0
2: Starting timer main with ID 0
2: Yielding timer main with ID 0
2: Created timer A() with ID 1 and parents [0]
2: Scheduling timer A() with ID 1
2: Starting timer A() with ID 1
2: Created timer B() with ID 2 and parents [0,1]
2: Scheduling timer B() with ID 2
2: Starting timer B() with ID 2
2: Stopping timer B() with ID 2
2: Destroying timer B() with ID 2 and parents [0,1] and children []
2: Created timer C() with ID 3 and parents [0,1]
2: Scheduling timer C() with ID 3
2: Starting timer C() with ID 3
2: Stopping timer C() with ID 3
2: Destroying timer C() with ID 3 and parents [0,1] and children []
2: Stopping timer A() with ID 1
2: Destroying timer A() with ID 1 and parents [0] and children [2,3]
2: Resuming timer main with ID 0
2: Stopping timer main with ID 0
2: Destroying timer main with ID 0 and parents [] and children [1,2,3]
2/2 Test #2: testWith .........................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 2

Total Test time (real) =   0.01 sec
```

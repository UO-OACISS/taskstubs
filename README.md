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
khuck@saturn:~/src/taskStubs$ ctest --test-dir build --verbose
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
2: Created timer 93927001593512 with ID 1 and parents [0]
2: Scheduling timer 93927001593512 with ID 1
2: Starting timer 93927001593512 with ID 1
2: Created timer 93927001593855 with ID 2 and parents [0,1]
2: Scheduling timer 93927001593855 with ID 2
2: Starting timer 93927001593855 with ID 2
2: Stopping timer 93927001593855 with ID 2
2: Destroying timer 93927001593855 with ID 2 and parents [0,1] and children []
2: Created timer 93927001594211 with ID 3 and parents [0,1]
2: Scheduling timer 93927001594211 with ID 3
2: Starting timer 93927001594211 with ID 3
2: Stopping timer 93927001594211 with ID 3
2: Destroying timer 93927001594211 with ID 3 and parents [0,1] and children []
2: Stopping timer 93927001593512 with ID 1
2: Destroying timer 93927001593512 with ID 1 and parents [0] and children [2,3]
2: Resuming timer main with ID 0
2: Stopping timer main with ID 0
2: Destroying timer main with ID 0 and parents [] and children [1,2,3]
2/2 Test #2: testWith .........................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 2

Total Test time (real) =   0.02 sec
```

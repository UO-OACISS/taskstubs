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
khuck@saturn:~/src/taskStubs$ rm -rf build ; cmake -B build -DCMAKE_BUILD_TYPE=Debug ; cmake --build build ; ctest --test-dir build --verbose
-- The C compiler identification is GNU 9.4.0
-- The CXX compiler identification is GNU 9.4.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.5s)
-- Generating done (0.1s)
-- Build files have been written to: /home/users/khuck/src/taskStubs/build
[  7%] Building C object timer_plugin/CMakeFiles/timer_plugin.dir/tasktimer.c.o
[ 15%] Building C object timer_plugin/CMakeFiles/timer_plugin.dir/plugin.c.o
[ 23%] Linking C static library libtimer_plugin.a
[ 23%] Built target timer_plugin
[ 30%] Building C object tool_example/CMakeFiles/MyToolC.dir/my_tool.c.o
[ 38%] Linking C shared library libMyToolC.so
[ 38%] Built target MyToolC
[ 46%] Building C object examples/CMakeFiles/main_c_without.dir/main.c.o
[ 53%] Linking C executable main_c_without
[ 53%] Built target main_c_without
[ 61%] Building C object examples/CMakeFiles/main_c_with.dir/main.c.o
[ 69%] Linking C executable main_c_with
[ 69%] Built target main_c_with
[ 76%] Building CXX object examples/CMakeFiles/main_cpp_without.dir/main.cpp.o
[ 84%] Linking CXX executable main_cpp_without
[ 84%] Built target main_cpp_without
[ 92%] Building CXX object examples/CMakeFiles/main_cpp_with.dir/main.cpp.o
[100%] Linking CXX executable main_cpp_with
[100%] Built target main_cpp_with
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
    Start 1: testWithout_c

1: Test command: /home/users/khuck/src/taskStubs/build/examples/main_c_without
1: Working Directory: /home/users/khuck/src/taskStubs/build/examples
1: Test timeout computed to be: 10000000
1/6 Test #1: testWithout_c ....................   Passed    0.00 sec
test 2
    Start 2: testWithout_c2

2: Test command: /home/users/khuck/src/taskStubs/build/examples/main_c_with
2: Working Directory: /home/users/khuck/src/taskStubs/build/examples
2: Test timeout computed to be: 10000000
2: in tasktimer_initialize
2: in tasktimer_create
2: in tasktimer_schedule
2: in tasktimer_start
2: in tasktimer_yield
2: in tasktimer_create
2: in tasktimer_schedule
2: in tasktimer_start
2: in tasktimer_create
2: in tasktimer_schedule
2: in tasktimer_start
2: in tasktimer_stop
2: in tasktimer_create
2: in tasktimer_schedule
2: in tasktimer_start
2: in tasktimer_stop
2: in tasktimer_stop
2: in tasktimer_resume
2: in tasktimer_stop
2: in tasktimer_finalize
2/6 Test #2: testWithout_c2 ...................   Passed    0.00 sec
test 3
    Start 3: testWith_c

3: Test command: /home/users/khuck/src/taskStubs/build/examples/main_c_with
3: Working Directory: /home/users/khuck/src/taskStubs/build/examples
3: Environment variables:
3:  TASKTIMER_EXTERNAL_TOOL_LIBRARY=/home/users/khuck/src/taskStubs/build/tool_example/libMyToolC.so
3: Test timeout computed to be: 10000000
3: in tasktimer_initialize
3: Initializing tool
3: in tasktimer_create
3: Created timer main with ID 0 and parents []
3: in tasktimer_schedule
3: Scheduling timer main with ID 0
3: in tasktimer_start
3: Starting timer main with ID 0
3: in tasktimer_yield
3: Yielding timer main with ID 0
3: in tasktimer_create
3: Created timer A with ID 1 and parents [0]
3: in tasktimer_schedule
3: Scheduling timer A with ID 1
3: in tasktimer_start
3: Starting timer A with ID 1
3: in tasktimer_create
3: Created timer B with ID 2 and parents [0,1]
3: in tasktimer_schedule
3: Scheduling timer B with ID 2
3: in tasktimer_start
3: Starting timer B with ID 2
3: in tasktimer_stop
3: Stopping timer B with ID 2
3: in tasktimer_create
3: Created timer C with ID 3 and parents [0,1]
3: in tasktimer_schedule
3: Scheduling timer C with ID 3
3: in tasktimer_start
3: Starting timer C with ID 3
3: in tasktimer_stop
3: Stopping timer C with ID 3
3: in tasktimer_stop
3: Stopping timer A with ID 1
3: in tasktimer_resume
3: Resuming timer main with ID 0
3: in tasktimer_stop
3: Stopping timer main with ID 0
3: in tasktimer_finalize
3: Finalizing tool
3/6 Test #3: testWith_c .......................   Passed    0.00 sec
test 4
    Start 4: testWithout_cpp

4: Test command: /home/users/khuck/src/taskStubs/build/examples/main_cpp_without
4: Working Directory: /home/users/khuck/src/taskStubs/build/examples
4: Test timeout computed to be: 10000000
4/6 Test #4: testWithout_cpp ..................   Passed    0.00 sec
test 5
    Start 5: testWithout_cpp2

5: Test command: /home/users/khuck/src/taskStubs/build/examples/main_cpp_with
5: Working Directory: /home/users/khuck/src/taskStubs/build/examples
5: Test timeout computed to be: 10000000
5: in tasktimer_initialize
5: in tasktimer_create
5: in tasktimer_schedule
5: in tasktimer_start
5: in tasktimer_yield
5: in tasktimer_create
5: in tasktimer_schedule
5: in tasktimer_start
5: in tasktimer_create
5: in tasktimer_schedule
5: in tasktimer_start
5: in tasktimer_stop
5: in tasktimer_create
5: in tasktimer_schedule
5: in tasktimer_start
5: in tasktimer_stop
5: in tasktimer_stop
5: in tasktimer_resume
5: in tasktimer_stop
5: in tasktimer_finalize
5/6 Test #5: testWithout_cpp2 .................   Passed    0.00 sec
test 6
    Start 6: testWith_cpp

6: Test command: /home/users/khuck/src/taskStubs/build/examples/main_cpp_with
6: Working Directory: /home/users/khuck/src/taskStubs/build/examples
6: Environment variables:
6:  TASKTIMER_EXTERNAL_TOOL_LIBRARY=/home/users/khuck/src/taskStubs/build/tool_example/libMyToolC.so
6: Test timeout computed to be: 10000000
6: in tasktimer_initialize
6: Initializing tool
6: in tasktimer_create
6: Created timer main with ID 0 and parents []
6: in tasktimer_schedule
6: Scheduling timer main with ID 0
6: in tasktimer_start
6: Starting timer main with ID 0
6: in tasktimer_yield
6: Yielding timer main with ID 0
6: in tasktimer_create
6: Created timer A with ID 1 and parents [0]
6: in tasktimer_schedule
6: Scheduling timer A with ID 1
6: in tasktimer_start
6: Starting timer A with ID 1
6: in tasktimer_create
6: Created timer B with ID 2 and parents [0,1]
6: in tasktimer_schedule
6: Scheduling timer B with ID 2
6: in tasktimer_start
6: Starting timer B with ID 2
6: in tasktimer_stop
6: Stopping timer B with ID 2
6: in tasktimer_create
6: Created timer _Z1Cv with ID 3 and parents [0,1]
6: in tasktimer_schedule
6: Scheduling timer _Z1Cv with ID 3
6: in tasktimer_start
6: Starting timer _Z1Cv with ID 3
6: in tasktimer_stop
6: Stopping timer _Z1Cv with ID 3
6: in tasktimer_stop
6: Stopping timer A with ID 1
6: in tasktimer_resume
6: Resuming timer main with ID 0
6: in tasktimer_stop
6: Stopping timer main with ID 0
6: in tasktimer_finalize
6: Finalizing tool
6/6 Test #6: testWith_cpp .....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 6

Total Test time (real) =   0.01 sec
```

/*
 * Copyright (c) 2014-2021 Kevin Huck
 * Copyright (c) 2014-2021 University of Oregon
 *
 * Distributed under the BSD 2-Clause Software License. (See accompanying
 * file LICENSE)
 */

#include <stddef.h>
#include "tasktimer.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string.h>

/* This simple example is truly overkill, but it tests all aspects of the API. */

uint64_t gettid(void) {
    pid_t x = syscall(__NR_gettid);
    return (uint64_t)(x);
}

uint64_t new_guid(void) {
    static uint64_t count = 0;
    return count++;
}

void A(uint64_t);
void B(uint64_t, uint64_t);
void C(uint64_t, uint64_t);
void D(void);
void E(void);
void F(void);
void xfer(void);

void A(uint64_t parent) {
    uint64_t parents[] = {parent};
    uint64_t myguid = new_guid();
    // both address and name
    TASKTIMER_CREATE(&A, "A", myguid, parents, 1, tt_A);
    tasktimer_argument_value_t args[1];
    args[0].type = TASKTIMER_LONG_INTEGER_TYPE;
    args[0].l_value = parent;
    TASKTIMER_SCHEDULE(tt_A, args, 1);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = gettid();
    TASKTIMER_START(tt_A, &resource);
    B(parent, myguid);
    C(parent, myguid);
    TASKTIMER_STOP(tt_A);
}

void B(uint64_t parent1, uint64_t parent2) {
    uint64_t parents[] = {parent1, parent2};
    uint64_t myguid = new_guid();
    // both address and name
    TASKTIMER_CREATE(&B, "B", myguid, parents, 2, tt_B);
    tasktimer_argument_value_t args[2];
    args[0].type = TASKTIMER_LONG_INTEGER_TYPE;
    args[0].l_value = parent1;
    args[1].type = TASKTIMER_LONG_INTEGER_TYPE;
    args[1].l_value = parent2;
    TASKTIMER_SCHEDULE(tt_B, args, 2);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = gettid();
    TASKTIMER_START(tt_B, &resource);
    TASKTIMER_STOP(tt_B);
}

void C(uint64_t parent1, uint64_t parent2) {
    uint64_t parents[] = {parent1, parent2};
    uint64_t myguid = new_guid();
    // no name, just address
    TASKTIMER_CREATE(&C, NULL, myguid, parents, 2, tt_C);
    tasktimer_argument_value_t args[2];
    args[0].type = TASKTIMER_LONG_INTEGER_TYPE;
    args[0].l_value = parent1;
    args[1].type = TASKTIMER_LONG_INTEGER_TYPE;
    args[1].l_value = parent2;
    TASKTIMER_SCHEDULE(tt_C, args, 2);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = gettid();
    TASKTIMER_START(tt_C, &resource);
    D();
    xfer();
    E();
    xfer();
    F();
    TASKTIMER_STOP(tt_C);
}

void D(void) {
    TASKTIMER_COMMAND_START(__func__);
    TASKTIMER_COMMAND_STOP();
}

void E(void) {
    TASKTIMER_COMMAND_START(__func__);
    TASKTIMER_COMMAND_STOP();
}

void F(void) {
    TASKTIMER_COMMAND_START(__func__);
    TASKTIMER_COMMAND_STOP();
}

void xfer(void) {
    const uint64_t maxlen = 1024;
    uint64_t source[maxlen];
    uint64_t dest[maxlen];
    memset(&source[0], 1, sizeof(uint64_t) * 1024);
    memset(&dest[0], 0, sizeof(uint64_t) * 1024);
    tasktimer_execution_space_t source_info, dest_info;
    tasktimer_execution_space_p sip = &source_info;
    tasktimer_execution_space_p dip = &dest_info;
    source_info.type = TASKTIMER_DEVICE_CPU;
    source_info.device_id = 0;
    source_info.instance_id = 0;
    dest_info.type = TASKTIMER_DEVICE_CPU;
    dest_info.device_id = 0;
    dest_info.instance_id = 0;
    TASKTIMER_DATA_TRANSFER_START(100, sip, "source", &source[0], dip, "dest", &dest[0]);
    memcpy(&dest[0], &source[0], sizeof(uint64_t) * maxlen);
    TASKTIMER_DATA_TRANSFER_STOP(100);
}

int main(int argc, char * argv[]) {
    // initialize the timer plugin
    TASKTIMER_INITIALIZE();
    uint64_t myguid = new_guid();
    // no address, just name
    TASKTIMER_CREATE(NULL, "main", myguid, NULL, 0, tt);
    // schedule the task
    TASKTIMER_SCHEDULE(tt, NULL, 0);
    // execute the task on CPU 0, thread_id
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = gettid();
    TASKTIMER_START(tt, &resource);
    // yield the task
    TASKTIMER_YIELD(tt);
    // run a "child" task
    A(myguid);
    // resume the task
    TASKTIMER_RESUME(tt, &resource);
    // stop the task
    TASKTIMER_STOP(tt);
    // finalize the timer plugin
    TASKTIMER_FINALIZE();
    return 0;
}

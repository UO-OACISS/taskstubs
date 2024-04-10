/*
 * Copyright (c) 2014-2021 Kevin Huck
 * Copyright (c) 2014-2021 University of Oregon
 *
 * Distributed under the BSD 2-Clause Software License. (See accompanying
 * file LICENSE)
 */

#include <stddef.h>
#include <atomic>
#include "tasktimer.h"

std::atomic<uint64_t> guid{0};

void A(uint64_t);
void B(uint64_t, uint64_t);
void C(uint64_t, uint64_t);

void A(uint64_t parent) {
    uint64_t parents[] = {parent};
    uint64_t myguid = guid++;
    // both address and name
    TASKTIMER_CREATE(&A, "A", myguid, parents, 1, tt_A);
    tasktimer_argument_value_t args[1];
    args[0].type = TASKTIMER_LONG_INTEGER_TYPE;
    args[0].l_value = parent;
    TASKTIMER_SCHEDULE(tt_A, args, 1);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt_A, &resource);
    B(parent, myguid);
    C(parent, myguid);
    TASKTIMER_STOP(tt_A);
}

void B(uint64_t parent1, uint64_t parent2) {
    uint64_t parents[] = {parent1, parent2};
    uint64_t myguid = guid++;
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
    resource.instance_id = 0;
    TASKTIMER_START(tt_B, &resource);
    TASKTIMER_STOP(tt_B);
}

void C(uint64_t parent1, uint64_t parent2) {
    uint64_t parents[] = {parent1, parent2};
    uint64_t myguid = guid++;
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
    resource.instance_id = 0;
    TASKTIMER_START(tt_C, &resource);
    TASKTIMER_STOP(tt_C);
}

int main(int argc, char * argv[]) {
    uint64_t myguid = guid++;
    // no address, just name
    TASKTIMER_INITIALIZE();
    TASKTIMER_CREATE(NULL, "main", myguid, NULL, 0, tt);
    TASKTIMER_SCHEDULE(tt, NULL, 0);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt, &resource);
    TASKTIMER_YIELD(tt);
    A(myguid);
    TASKTIMER_RESUME(tt, &resource);
    TASKTIMER_STOP(tt);
    TASKTIMER_FINALIZE();
    return 0;
}

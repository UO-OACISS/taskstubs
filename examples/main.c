/*
 * Copyright (c) 2014-2021 Kevin Huck
 * Copyright (c) 2014-2021 University of Oregon
 *
 * Distributed under the BSD 2-Clause Software License. (See accompanying
 * file LICENSE)
 */

#include <stddef.h>
#include "tasktimer.h"

void A(void);
void B(void);
void C(void);

void A(void) {
    uint64_t parents[] = {0};
    // both address and name
    TASKTIMER_CREATE(&A, "A", 1, parents, 1, tt_A);
    TASKTIMER_SCHEDULE(tt_A, NULL, 0);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt_A, &resource);
    B();
    C();
    TASKTIMER_STOP(tt_A);
}

void B(void) {
    uint64_t parents[] = {0,1};
    // both address and name
    TASKTIMER_CREATE(&B, "B", 2, parents, 2, tt_B);
    TASKTIMER_SCHEDULE(tt_B, NULL, 0);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt_B, &resource);
    TASKTIMER_STOP(tt_B);
}

void C(void) {
    uint64_t parents[] = {0,1};
    // no name, just address
    TASKTIMER_CREATE(&C, NULL, 3, parents, 2, tt_C);
    TASKTIMER_SCHEDULE(tt_C, NULL, 0);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt_C, &resource);
    TASKTIMER_STOP(tt_C);
}

int main(int argc, char * argv[]) {
    // no address, just name
    TASKTIMER_INITIALIZE();
    TASKTIMER_CREATE(NULL, "main", 0, NULL, 0, tt);
    TASKTIMER_SCHEDULE(tt, NULL, 0);
    tasktimer_execution_space_t resource;
    resource.type = TASKTIMER_DEVICE_CPU;
    resource.device_id = 0;
    resource.instance_id = 0;
    TASKTIMER_START(tt, &resource);
    TASKTIMER_YIELD(tt);
    A();
    TASKTIMER_RESUME(tt, &resource);
    TASKTIMER_STOP(tt);
    TASKTIMER_FINALIZE();
    return 0;
}

#include <stddef.h>
#include "TimerPlugin/tasktimer.h"

void A(void);
void B(void);
void C(void);

void A(void) {
    uint64_t parents[] = {0};
    tasktimer_t tt_A = tasktimer_create_address((uintptr_t)&A, 1, parents, 1);
    tasktimer_schedule(tt_A);
    tasktimer_start(tt_A);
    B();
    C();
    tasktimer_stop(tt_A);
}

void B(void) {
    uint64_t parents[] = {0,1};
    tasktimer_t tt_B = tasktimer_create_address((uintptr_t)&B, 2, parents, 2);
    tasktimer_schedule(tt_B);
    tasktimer_start(tt_B);
    tasktimer_stop(tt_B);
}

void C(void) {
    uint64_t parents[] = {0,1};
    tasktimer_t tt_C = tasktimer_create_address((uintptr_t)&C, 3, parents, 2);
    tasktimer_schedule(tt_C);
    tasktimer_start(tt_C);
    tasktimer_stop(tt_C);
}

int main(int argc, char * argv[]) {
    tasktimer_t tt = tasktimer_create_string("main", 0, NULL, 0);
    tasktimer_schedule(tt);
    tasktimer_start(tt);
    tasktimer_yield(tt);
    A();
    tasktimer_resume(tt);
    tasktimer_stop(tt);
    return 0;
}

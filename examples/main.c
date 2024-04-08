#include <stddef.h>
#include "tasktimer.h"

void A(void);
void B(void);
void C(void);

void A(void) {
    uint64_t parents[] = {0};
    // both address and name
    TASKTIMER_CREATE(&A, "A", 1, parents, 1, tt_A);
    TASKTIMER_SCHEDULE(tt_A);
    TASKTIMER_START(tt_A);
    B();
    C();
    TASKTIMER_STOP(tt_A);
}

void B(void) {
    uint64_t parents[] = {0,1};
    // both address and name
    TASKTIMER_CREATE(&B, "B", 2, parents, 2, tt_B);
    TASKTIMER_SCHEDULE(tt_B);
    TASKTIMER_START(tt_B);
    TASKTIMER_STOP(tt_B);
}

void C(void) {
    uint64_t parents[] = {0,1};
    // no name, just address
    TASKTIMER_CREATE(&C, NULL, 3, parents, 2, tt_C);
    TASKTIMER_SCHEDULE(tt_C);
    TASKTIMER_START(tt_C);
    TASKTIMER_STOP(tt_C);
}

int main(int argc, char * argv[]) {
    // no address, just name
    TASKTIMER_INITIALIZE();
    TASKTIMER_CREATE(NULL, "main", 0, NULL, 0, tt);
    TASKTIMER_SCHEDULE(tt);
    TASKTIMER_START(tt);
    TASKTIMER_YIELD(tt);
    A();
    TASKTIMER_RESUME(tt);
    TASKTIMER_STOP(tt);
    TASKTIMER_FINALIZE();
    return 0;
}

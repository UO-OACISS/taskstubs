#include <iostream>

#include "TimerPlugin/TaskTimer.hpp"

void A(void);
void B(void);
void C(void);

void A(void) {
    TaskTimer tt_A{(uintptr_t)&A, 1, {0}};
    tt_A.schedule();
    tt_A.start();
    B();
    C();
    tt_A.stop();
}

void B(void) {
    TaskTimer tt_B{(uintptr_t)&B, 2, {0,1}};
    tt_B.schedule();
    tt_B.start();
    tt_B.stop();
}

void C(void) {
    TaskTimer tt_C{(uintptr_t)&C, 3, {0,1}};
    tt_C.schedule();
    tt_C.start();
    tt_C.stop();
}

int main(int argc, char * argv[]) {
    TaskTimer tt{"main", 0};
    tt.schedule();
    tt.start();
    tt.yield();
    A();
    tt.resume();
    tt.stop();
    return 0;
}

#include "tasktimer.h"
#include "TaskTimer.hpp"

extern "C" {
/* Task lifetime */
tasktimer_t tasktimer_create_string(const char* name, const uint64_t id,
    const uint64_t* parents, const uint64_t count) {
    std::set<uint64_t> parentSet;
    for (uint64_t i = 0 ; i < count; i++) {
        parentSet.insert(parents[i]);
    }
    TaskTimer* tt = new TaskTimer{std::string(name), id, parentSet};
    return tt;
}
tasktimer_t tasktimer_create_address(const uintptr_t address, const uint64_t id,
    const uint64_t* parents, const uint64_t count) {
    std::set<uint64_t> parentSet;
    for (uint64_t i = 0; i < count; i++) {
        parentSet.insert(parents[i]);
    }
    TaskTimer* tt = new TaskTimer{address, id, parentSet};
    return tt;
}
void tasktimer_schedule(tasktimer_t timer) {
    TaskTimer* tt = reinterpret_cast<TaskTimer*>(timer);
    tt->schedule();
}
void tasktimer_start(tasktimer_t timer) {
    TaskTimer* tt = reinterpret_cast<TaskTimer*>(timer);
    tt->start();
}
void tasktimer_yield(tasktimer_t timer) {
    TaskTimer* tt = reinterpret_cast<TaskTimer*>(timer);
    tt->yield();
}
void tasktimer_resume(tasktimer_t timer) {
    TaskTimer* tt = reinterpret_cast<TaskTimer*>(timer);
    tt->resume();
}
void tasktimer_stop(tasktimer_t timer) {
    TaskTimer* tt = reinterpret_cast<TaskTimer*>(timer);
    tt->stop();
    /* Is this the right place? */
    delete(tt);
}
void tasktimer_addParents(tasktimer_t timer, const uint64_t* parents, const uint64_t count) {
    std::set<uint64_t> parentSet;
    for (uint64_t i = 0; i < count; i++) {
        parentSet.insert(parents[i]);
    }
    TaskTimer* tt = reinterpret_cast<TaskTimer*>(timer);
    tt->addParents(parentSet);
}
void tasktimer_addChildren(tasktimer_t timer, const uint64_t* children, const uint64_t count) {
    std::set<uint64_t> childSet;
    for (uint64_t i = 0; i < count; i++) {
        childSet.insert(children[i]);
    }
    TaskTimer* tt = reinterpret_cast<TaskTimer*>(timer);
    tt->addParents(childSet);
}

}

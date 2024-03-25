#pragma once
#include <stdint.h>

typedef void* tasktimer_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Task lifetime */
tasktimer_t tasktimer_create_string(const char* name, const uint64_t id,
    const uint64_t* parents, const uint64_t count);
tasktimer_t tasktimer_create_address(const uintptr_t address, const uint64_t id,
    const uint64_t* parents, const uint64_t count);
void tasktimer_schedule(tasktimer_t timer);
void tasktimer_start(tasktimer_t timer);
void tasktimer_yield(tasktimer_t timer);
void tasktimer_resume(tasktimer_t timer);
void tasktimer_stop(tasktimer_t timer);
void tasktimer_addParents(tasktimer_t timer, const uint64_t* parents, const uint64_t count);
void tasktimer_addChildren(tasktimer_t timer, const uint64_t* children, const uint64_t count);

#ifdef __cplusplus
}
#endif

#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "tool_api.h"

typedef struct my_timer {
        char* mName;
        uint64_t mID;
        int hasName;
        const void* mAddress;
        /* This is a dumb example with fixed lengths for parents, children */
        uint64_t mParents[32];
        uint64_t mParentCount;
        uint64_t mChildren[32];
        uint64_t mChildCount;
} my_timer_t;

static char* label(my_timer_t* t) {
    if (t->hasName == 1) return t->mName;
    Dl_info this_fn_info;
    if (dladdr((const void*)(t->mAddress), &this_fn_info)) {
        int status = 0;
        t->mName = strdup(this_fn_info.dli_sname);
        t->hasName = 1;
    }
    return t->mName;
}

#define my_min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

static char* printSet(const uint64_t* inSet, const uint64_t count) {
    char ss[1024] = {"["};
    int first = 1;
    for (uint64_t i = 0 ; i < count ; i++) {
        if (first == 0) strcat(ss, ",");
        char tmp[64];
        sprintf(tmp, "%" PRId64 "", inSet[i]);
        int minlen = my_min(1024 - strlen(ss), strlen(tmp));
        strncat(ss, tmp, minlen);
        first = 0;
    }
    strcat(ss, "]");
    return strdup(ss);
}

/* not that sophisticated, just append the new set to the old set */
static uint64_t merge_set(uint64_t* old_set, uint64_t old_count, const uint64_t* new_set, const uint64_t new_count) {
    if (new_count > 0) {
        for(uint64_t i = 0 ; i < new_count ; i++) {
            old_set[old_count + i] = new_set[i];
        }
    }
    return old_count + new_count;
}

static my_timer_t* create_mytimer(const void* address, const char* name,
    const uint64_t id, const uint64_t* parents, const uint64_t count) {
    my_timer_t* timer = (my_timer_t*)malloc(sizeof(my_timer_t));
    timer->mAddress = address;
    if (name != NULL && strlen(name) > 0) {
        timer->mName = strdup(name);
        timer->hasName = 1;
    } else {
        timer->hasName = 0;
    }
    timer->mID = id;
    timer->mParentCount = merge_set(timer->mParents, 0, parents, count);
    printf("Created timer %s with ID %" PRId64 " and parents %s\n",
        label(timer), timer->mID, printSet(timer->mParents, timer->mParentCount));
    return timer;
}

static void init(void) {
    printf("Initializing tool\n");
}

static void finalize(void) {
    printf("Finalizing tool\n");
}

static void schedule(my_timer_t* timer) {
    printf("Scheduling timer %s with ID %" PRId64 "\n", label(timer), timer->mID);
}

static void start(my_timer_t* timer) {
    printf("Starting timer %s with ID %" PRId64 "\n", label(timer), timer->mID);
}

static void yield(my_timer_t* timer) {
    printf("Yielding timer %s with ID %" PRId64 "\n", label(timer), timer->mID);
}

static void resume(my_timer_t* timer) {
    printf("Resuming timer %s with ID %" PRId64 "\n", label(timer), timer->mID);
}

static void stop(my_timer_t* timer) {
    printf("Stopping timer %s with ID %" PRId64 "\n", label(timer), timer->mID);
}

static void add_parents(my_timer_t* timer, const uint64_t* parents, const uint64_t count) {
    timer->mParentCount = merge_set(timer->mParents, timer->mParentCount, parents, count);
    printf("Timer %s adding parents %s\n", label(timer), printSet(timer->mParents, count));
}

static void add_children(my_timer_t* timer, const uint64_t* children, const uint64_t count) {
    timer->mChildCount = merge_set(timer->mChildren, timer->mChildCount, children, count);
    printf("Timer %s adding children %s\n", label(timer), printSet(timer->mChildren, count));
}

void tasktimer_impl_initialize(void) {
    init();
}

void tasktimer_impl_finalize(void) {
    finalize();
}

tasktimer_timer_t tasktimer_impl_create(const tasktimer_function_pointer_t address,
    const char* name, const uint64_t id, const uint64_t* parents,
    const uint64_t count) {
    my_timer_t* tmp = create_mytimer(address, name, id, parents, count);
    return tmp;
}

void tasktimer_impl_destroy(tasktimer_timer_t timer) {
    free(timer);
}

void tasktimer_impl_schedule(tasktimer_timer_t in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    schedule(timer);
}

void tasktimer_impl_start(tasktimer_timer_t in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    start(timer);
}

void tasktimer_impl_yield(tasktimer_timer_t in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    yield(timer);
}

void tasktimer_impl_resume(tasktimer_timer_t in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    resume(timer);
}

void tasktimer_impl_stop(tasktimer_timer_t in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    stop(timer);
}

void tasktimer_impl_add_parents(tasktimer_timer_t in_timer,
    const uint64_t* parents, const uint64_t count) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    add_parents(timer, parents, count);
}

void tasktimer_impl_add_children(tasktimer_timer_t in_timer,
    const uint64_t* children, const uint64_t count) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    add_children(timer, children, count);
}


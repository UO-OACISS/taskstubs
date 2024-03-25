#include "TimerPlugin/tool_api.h"
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct my_timer {
        char* mName;
        uint64_t mID;
        int hasName;
        uintptr_t mAddress;
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
        t->hasName = true;
    }
    return t->mName;
}

static char* printSet(const uint64_t* inSet, const uint64_t count) {
    char ss[1024] = {"["};
    int first = 1;
    for (uint64_t i = 0 ; i < count ; i++) {
        if (first == 0) strncat(ss, ",", 1);
        char tmp[64];
        sprintf(tmp, "%" PRId64 "", inSet[i]);
        strncat(ss, tmp, strlen(tmp));
        first = 0;
    }
    strncat(ss, "]", 1);
    return strdup(ss);
}

/* not that sophisticated, just append the new set to the old set */
static uint64_t merge_set(uint64_t* old_set, uint64_t old_count, const uint64_t* new_set, const uint64_t new_count) {
    for(uint64_t i = 0 ; i < new_count ; i++) {
        old_set[old_count + i] = new_set[i];
    }
    return old_count + new_count;
}

static my_timer_t* mytimer_string(const char* name, const uint64_t id,
            const uint64_t* parents, const uint64_t count) {
    my_timer_t* timer = (my_timer_t*)malloc(sizeof(my_timer_t));
    timer->mName = strdup(name);
    timer->hasName = 1;
    timer->mID = id;
    timer->mParentCount = merge_set(timer->mParents, 0, parents, count);
    printf("Created timer %s with ID %" PRId64 " and parents %s\n",
        label(timer), timer->mID, printSet(timer->mParents, timer->mParentCount));
    return timer;
}

static my_timer_t* mytimer_address(const uintptr_t address, const uint64_t id,
            const uint64_t* parents, const uint64_t count) {
    my_timer_t* timer = (my_timer_t*)malloc(sizeof(my_timer_t));
    timer->mAddress = address;
    timer->hasName = 0;
    timer->mID = id;
    timer->mParentCount = merge_set(timer->mParents, 0, parents, count);
    printf("Created timer %s with ID %" PRId64 " and parents %s\n",
        label(timer), timer->mID, printSet(timer->mParents, timer->mParentCount));
    return timer;
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

void* tasktimer_impl_create_string(const char* name, const uint64_t id,
    const uint64_t* parents, const uint64_t count) {
    my_timer_t* tmp = mytimer_string(name, id, parents, count);
    return tmp;
}

void* tasktimer_impl_create_address(const uintptr_t address, const uint64_t id,
    const uint64_t* parents, const uint64_t count) {
    my_timer_t* tmp = mytimer_address(address, id, parents, count);
    return tmp;
}

void tasktimer_impl_schedule(void* in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    schedule(timer);
}

void tasktimer_impl_start(void* in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    start(timer);
}

void tasktimer_impl_yield(void* in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    yield(timer);
}

void tasktimer_impl_resume(void* in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    resume(timer);
}

void tasktimer_impl_stop(void* in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    stop(timer);
}

void tasktimer_impl_add_parents(void* in_timer,
    const uint64_t* parents, const uint64_t count) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    add_parents(timer, parents, count);
}

void tasktimer_impl_add_children(void* in_timer,
    const uint64_t* children, const uint64_t count) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    add_children(timer, children, count);
}


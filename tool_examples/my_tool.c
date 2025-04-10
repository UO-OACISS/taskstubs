/*
 * Copyright (c) 2014-2021 Kevin Huck
 * Copyright (c) 2014-2021 University of Oregon
 *
 * Distributed under the BSD 2-Clause Software License. (See accompanying
 * file LICENSE)
 */

#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "tool_api.h"

#define MAX_PARENTS  32
#define MAX_CHILDREN 128

typedef struct my_timer {
        char* mName;
        uint64_t mID;
        int hasName;
        const void* mAddress;
        /* This is a dumb example with fixed lengths for parents, children */
        uint64_t mParents[MAX_PARENTS];
        uint64_t mParentCount;
        uint64_t mChildren[MAX_CHILDREN];
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
static uint64_t merge_set(uint64_t* old_set, uint64_t old_count, const uint64_t* new_set, const uint64_t new_count, const uint64_t max_count) {
    if (new_count > 0) {
        for(uint64_t i = 0 ; i < new_count ; i++) {
            if(old_count + i >= max_count) {
                fprintf(stderr, "%s:%d Exceeded number of children or parents in merge_set\n", __FILE__, __LINE__);
                return old_count+i;
            }
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
    timer->mParentCount = merge_set(timer->mParents, 0, parents, count, MAX_PARENTS);
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
    timer->mParentCount = merge_set(timer->mParents, timer->mParentCount, parents, count, MAX_PARENTS);
    printf("Timer %s adding parents %s\n", label(timer), printSet(timer->mParents, count));
}

static void add_children(my_timer_t* timer, const uint64_t* children, const uint64_t count) {
    timer->mChildCount = merge_set(timer->mChildren, timer->mChildCount, children, count, MAX_CHILDREN);
    printf("Timer %s adding children %s\n", label(timer), printSet(timer->mChildren, count));
}

static void data_transfer_start(tasktimer_guid_t guid,
    tasktimer_execution_space_p source_type, const char* source_name, const void* source_ptr,
    tasktimer_execution_space_p dest_type, const char* dest_name, const void* dest_ptr) {
    printf("Data transfer %lu start\n", guid);
}

static void data_transfer_stop(tasktimer_guid_t guid) {
    printf("Data transfer %lu stop\n", guid);
}

static void command_start(const char* type_name) {
    printf("Command %s start\n", type_name);
}

static void command_stop() {
    printf("Command stop\n");
}

static void sample_value(const char* name, const double value) {
    printf("sample value %s = %f\n", name, value);
}

static void mark(const char* label) {
    printf("mark an event %s\n", label);
}

void tasktimer_initialize_impl(void) {
    init();
}

void tasktimer_finalize_impl(void) {
    finalize();
}

tasktimer_timer_t tasktimer_create_impl(const tasktimer_function_pointer_t address,
    const char* name, const uint64_t id, const uint64_t* parents,
    const uint64_t count) {
    my_timer_t* tmp = create_mytimer(address, name, id, parents, count);
    return tmp;
}

void tasktimer_destroy_impl(tasktimer_timer_t timer) {
    free(timer);
}

void tasktimer_schedule_impl(tasktimer_timer_t in_timer,
    tasktimer_argument_value_t* arguments, uint64_t argument_count) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    schedule(timer);
}

void tasktimer_start_impl(tasktimer_timer_t in_timer, tasktimer_execution_space_p resource) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    start(timer);
}

void tasktimer_yield_impl(tasktimer_timer_t in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    yield(timer);
}

void tasktimer_resume_impl(tasktimer_timer_t in_timer, tasktimer_execution_space_p resource) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    resume(timer);
}

void tasktimer_stop_impl(tasktimer_timer_t in_timer) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    stop(timer);
}

void tasktimer_add_parents_impl(tasktimer_timer_t in_timer,
    const uint64_t* parents, const uint64_t count) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    add_parents(timer, parents, count);
}

void tasktimer_add_children_impl(tasktimer_timer_t in_timer,
    const uint64_t* children, const uint64_t count) {
    my_timer_t* timer = (my_timer_t*)(in_timer);
    add_children(timer, children, count);
}

void tasktimer_data_transfer_start_impl(tasktimer_guid_t guid,
    tasktimer_execution_space_p source_type, const char* source_name, const void* source_ptr,
    tasktimer_execution_space_p dest_type, const char* dest_name, const void* dest_ptr) {
    data_transfer_start(guid, source_type, source_name, source_ptr, dest_type, dest_name, dest_ptr);
}

void tasktimer_data_transfer_stop_impl(tasktimer_guid_t guid) {
    data_transfer_stop(guid);
}

void tasktimer_command_start_impl(const char* type_name) {
    command_start(type_name);
}

void tasktimer_command_stop_impl(void) {
    command_stop();
}

void tasktimer_sample_value_impl(const char* name, const double value) {
    sample_value(name, value);
}

void tasktimer_mark_impl(const char* label) {
    mark(label);
}



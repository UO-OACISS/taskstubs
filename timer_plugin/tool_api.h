#pragma once

#include <stdint.h>

/* object types */
typedef void* tasktimer_timer_t;
typedef void* tasktimer_function_pointer_t;
typedef uint64_t tasktimer_guid_t;

/* Function pointer types */
typedef void tasktimer_initialize_t(void);
typedef void tasktimer_finalize_t(void);
typedef tasktimer_timer_t tasktimer_create_t(const tasktimer_function_pointer_t,
    const char*, const tasktimer_guid_t, const tasktimer_guid_t*, const uint64_t);
typedef void tasktimer_schedule_t(tasktimer_timer_t);
typedef void tasktimer_start_t(tasktimer_timer_t);
typedef void tasktimer_yield_t(tasktimer_timer_t);
typedef void tasktimer_resume_t(tasktimer_timer_t);
typedef void tasktimer_stop_t(tasktimer_timer_t);
typedef void tasktimer_destroy_t(tasktimer_timer_t);
typedef void tasktimer_add_parents_t(tasktimer_timer_t, const tasktimer_guid_t*,
    const uint64_t);
typedef void tasktimer_add_children_t(tasktimer_timer_t, const tasktimer_guid_t*,
    const uint64_t);

#ifdef COMPILE_PLUGIN
#define PLUGIN_INTERFACE __attribute__((visibility("default")))
#else
#define PLUGIN_INTERFACE
#endif

/* Function declarations */
PLUGIN_INTERFACE tasktimer_initialize_t tasktimer_impl_initialize;
PLUGIN_INTERFACE tasktimer_finalize_t tasktimer_impl_finalize;
PLUGIN_INTERFACE tasktimer_create_t tasktimer_impl_create;
PLUGIN_INTERFACE tasktimer_schedule_t tasktimer_impl_schedule;
PLUGIN_INTERFACE tasktimer_start_t tasktimer_impl_start;
PLUGIN_INTERFACE tasktimer_yield_t tasktimer_impl_yield;
PLUGIN_INTERFACE tasktimer_resume_t tasktimer_impl_resume;
PLUGIN_INTERFACE tasktimer_stop_t tasktimer_impl_stop;
PLUGIN_INTERFACE tasktimer_destroy_t tasktimer_impl_destroy;
PLUGIN_INTERFACE tasktimer_add_parents_t tasktimer_impl_add_parents;
PLUGIN_INTERFACE tasktimer_add_children_t tasktimer_impl_add_children;


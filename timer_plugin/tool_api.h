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

/* Function declarations:
   the plugin loader will look for these, and
   the tool should implement them. */
PLUGIN_INTERFACE tasktimer_initialize_t tasktimer_initialize_impl;
PLUGIN_INTERFACE tasktimer_finalize_t tasktimer_finalize_impl;
PLUGIN_INTERFACE tasktimer_create_t tasktimer_create_impl;
PLUGIN_INTERFACE tasktimer_schedule_t tasktimer_schedule_impl;
PLUGIN_INTERFACE tasktimer_start_t tasktimer_start_impl;
PLUGIN_INTERFACE tasktimer_yield_t tasktimer_yield_impl;
PLUGIN_INTERFACE tasktimer_resume_t tasktimer_resume_impl;
PLUGIN_INTERFACE tasktimer_stop_t tasktimer_stop_impl;
PLUGIN_INTERFACE tasktimer_destroy_t tasktimer_destroy_impl;
PLUGIN_INTERFACE tasktimer_add_parents_t tasktimer_add_parents_impl;
PLUGIN_INTERFACE tasktimer_add_children_t tasktimer_add_children_impl;


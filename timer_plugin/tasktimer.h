/*
 * Copyright (c) 2014-2021 Kevin Huck
 * Copyright (c) 2014-2021 University of Oregon
 *
 * Distributed under the BSD 2-Clause Software License. (See accompanying
 * file LICENSE)
 */

#pragma once
#include <stdint.h>
#include <tool_api.h>

#if defined(TASKTIMER_USE_TIMERS)

#ifdef __cplusplus
extern "C" {
#endif

/* init/fini */
tasktimer_initialize_t tasktimer_initialize;
tasktimer_finalize_t tasktimer_finalize;
/* Task lifetime */
tasktimer_create_t tasktimer_create;
tasktimer_schedule_t tasktimer_schedule;
tasktimer_start_t tasktimer_start;
tasktimer_yield_t tasktimer_yield;
tasktimer_resume_t tasktimer_resume;
tasktimer_stop_t tasktimer_stop;
tasktimer_destroy_t tasktimer_destroy;
tasktimer_add_parents_t tasktimer_add_parents;
tasktimer_add_children_t tasktimer_add_children;
tasktimer_data_transfer_start_t tasktimer_data_transfer_start;
tasktimer_data_transfer_stop_t tasktimer_data_transfer_stop;

#ifdef __cplusplus
}
#endif

/* Macro API for option of entirely disabling at compile time
 * To use this API, set the Macro TASKTIMER_USE_TIMERS on the compile
 * line or in a config.h file, however your project does it
 */


/* Basic API - for documentation of these calls, see tool_api.h */

#define TASKTIMER_INITIALIZE() \
        tasktimer_initialize()
#define TASKTIMER_FINALIZE() \
        tasktimer_finalize()
#define TASKTIMER_CREATE(_address, _name, _guid, _parents, _parent_count, _timer) \
        tasktimer_timer_t _timer = tasktimer_create((tasktimer_function_pointer_t)_address, _name, _guid, _parents, _parent_count)
#define TASKTIMER_SCHEDULE(_timer, _args, _count) \
        tasktimer_schedule(_timer, _args, _count)
#define TASKTIMER_START(_timer, _resource) \
        tasktimer_start(_timer, _resource)
#define TASKTIMER_YIELD(_timer) \
        tasktimer_yield(_timer)
#define TASKTIMER_RESUME(_timer, _resource) \
        tasktimer_resume(_timer, _resource)
#define TASKTIMER_STOP(_timer) \
        tasktimer_stop(_timer)
#define TASKTIMER_DESTROY(_timer) \
        tasktimer_destroy(_timer)
#define TASKTIMER_ADD_PARENTS(_timer, _parents, _parent_count) \
        tasktimer_add_parents(_timer, _parents, _parent_count)
#define TASKTIMER_ADD_CHILDREN(_timer, _children, _child_count) \
        tasktimer_add_parents(_timer, _children, _child_count)
#define TASKTIMER_DATA_TRANSFER_START(_guid, _src_resource, _src_name, _src_ptr, _dest_resource, _dest_name, _dest_ptr) \
        tasktimer_data_transfer_start(_guid, _src_resource, _src_name, _src_ptr, _dest_resource, _dest_name, _dest_ptr)
#define TASKTIMER_DATA_TRANSFER_STOP(_guid) \
        tasktimer_data_transfer_stop(_guid)

#else

#define TASKTIMER_INITIALIZE()
#define TASKTIMER_FINALIZE()
#define TASKTIMER_CREATE(_address, _name, _guid, _parents, _parent_count, _timer)
#define TASKTIMER_SCHEDULE(_timer, _args, _count)
#define TASKTIMER_START(_timer, _resource)
#define TASKTIMER_YIELD(_timer)
#define TASKTIMER_RESUME(_timer, _resource)
#define TASKTIMER_STOP(_timer)
#define TASKTIMER_DESTROY(_timer)
#define TASKTIMER_ADD_PARENTS(_timer, _parents, _parent_count)
#define TASKTIMER_ADD_CHILDREN(_timer, _children, _child_count)
#define TASKTIMER_DATA_TRANSFER_START(_guid, _src_resource, _src_name, _src_ptr, _dest_resource, _dest_name, _dest_ptr)
#define TASKTIMER_DATA_TRANSFER_STOP(_guid)

#endif


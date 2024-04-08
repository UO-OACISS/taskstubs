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

#ifdef __cplusplus
}
#endif

/* Macro API for option of entirely disabling at compile time
 * To use this API, set the Macro TASKTIMER_USE_TIMERS on the compile
 * line or in a config.h file, however your project does it
 */


/* Basic API */

#define TASKTIMER_INITIALIZE() \
        tasktimer_initialize()
#define TASKTIMER_FINALIZE() \
        tasktimer_finalize()
#define TASKTIMER_CREATE(_address, _name, _guid, _parents, _parent_count, _timer) \
        tasktimer_timer_t _timer = tasktimer_create((tasktimer_function_pointer_t)_address, _name, _guid, _parents, _parent_count)
#define TASKTIMER_SCHEDULE(_timer) \
        tasktimer_schedule(_timer)
#define TASKTIMER_START(_timer) \
        tasktimer_start(_timer)
#define TASKTIMER_YIELD(_timer) \
        tasktimer_yield(_timer)
#define TASKTIMER_RESUME(_timer) \
        tasktimer_resume(_timer)
#define TASKTIMER_STOP(_timer) \
        tasktimer_stop(_timer)
#define TASKTIMER_DESTROY(_timer) \
        tasktimer_destroy(_timer)
#define TASKTIMER_ADD_PARENTS(_timer, _parents, _parent_count) \
        tasktimer_add_parents(_timer, _parents, _parent_count)
#define TASKTIMER_ADD_CHILDREN(_timer, _children, _child_count) \
        tasktimer_add_parents(_timer, _children, _child_count)

#else

#define TASKTIMER_INITIALIZE()
#define TASKTIMER_FINALIZE()
#define TASKTIMER_CREATE(_address, _name, _guid, _parents, _parent_count, _timer)
#define TASKTIMER_SCHEDULE(_timer)
#define TASKTIMER_START(_timer)
#define TASKTIMER_YIELD(_timer)
#define TASKTIMER_RESUME(_timer)
#define TASKTIMER_STOP(_timer)
#define TASKTIMER_DESTROY(_timer)
#define TASKTIMER_ADD_PARENTS(_timer, _parents, _parent_count)
#define TASKTIMER_ADD_CHILDREN(_timer, _children, _child_count)

#endif


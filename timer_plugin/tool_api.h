/*
 * Copyright (c) 2014-2021 Kevin Huck
 * Copyright (c) 2014-2021 University of Oregon
 *
 * Distributed under the BSD 2-Clause Software License. (See accompanying
 * file LICENSE)
 */

#pragma once

#include <stdint.h>

/* object types */

// Timer object
typedef void* tasktimer_timer_t;
// Generic function pointer
typedef void* tasktimer_function_pointer_t;
// Globally unique identifier for tasks
typedef uint64_t tasktimer_guid_t;

/* Enumeration for scheduler arguments */
typedef enum {
    TASKTIMER_LONG_INTEGER_TYPE = 0,
    TASKTIMER_DOUBLE_TYPE,
    TASKTIMER_STRING_TYPE,
    TASKTIMER_POINTER_TYPE,
    TASKTIMER_ARRAY_TYPE
} tasktimer_argument_type_t;

/* Way to capture arguments for the task passed to the scheduler
   - in order to uniquely identify / categorize tasks if desired */
typedef struct tasktimer_argument_value {
    tasktimer_argument_type_t type;
    union {
        uint64_t l_value;
        double d_value;
        char* c_value;
        void* p_value;
        void* a_value;
    };
} tasktimer_argument_value_t;

/* Enumeration for data transfers and task execution */
typedef enum device_type {
    TASKTIMER_DEVICE_CPU = 0,
    TASKTIMER_DEVICE_GPU,
    TASKTIMER_DEVICE_OTHER
} tasktimer_device_type_t;

/* Way to capture where the task is executing */
typedef struct tasktimer_execution_space_t {
    tasktimer_device_type_t type;
    uint64_t device_id; // cpu id, gpu id, etc.
    uint64_t instance_id; // stream id, queue id, etc.
} tasktimer_execution_space_t;

/* Convenience types for pointers to above structures */
typedef tasktimer_argument_value_t* tasktimer_argument_value_p;
typedef tasktimer_execution_space_t* tasktimer_execution_space_p;

/* Function pointer types */

/**
 \brief Initialize the performance tool

 The initialize function will perform whatever tool initialization is needed.
 \return no return value.
 \sa @ref tasktimer_finalize_t
*/
typedef void tasktimer_initialize_t(void);

/**
 \brief Finalize the performance tool

 The finalize function will perform whatever tool finalization is needed.
 \return no return value.
 \sa @ref tasktimer_initialize_t
*/
typedef void tasktimer_finalize_t(void);

/**
 \brief Create a task

 The task creation function will notify the tool that a task has been created.
 The memory allocated by the tool to store the timer will be freed when the
 tasktimer_destroy_t function is called.
 \param address the address of the task (function, lambda, functor...)
 \param name    the name of the task (optional)
 \param guid    the GUID of the task (optional)
 \param parents an array of parent GUIDs for this task
 \param count   the number of parent tasks for this task
 \return a pointer to the timer object to store with the task
 \sa @ref tasktimer_destroy_t
*/
typedef tasktimer_timer_t tasktimer_create_t(const tasktimer_function_pointer_t,
    const char*, const tasktimer_guid_t, const tasktimer_guid_t*, const uint64_t);

/**
 \brief Schedule a task

 The schedule function will notify that a task has been scheduled.
 \param timer     the tool-specific timer object that is associated with this task
 \param arguments array of arguments that are passed to the task (optional)
 \param count     number of arguments (optional)
 \sa @ref tasktimer_start_t
 \sa @ref tasktimer_yield_t
 \sa @ref tasktimer_resume_t
 \sa @ref tasktimer_stop_t
*/
typedef void tasktimer_schedule_t(tasktimer_timer_t, tasktimer_argument_value_p, uint64_t);

/**
 \brief Start a task

 The start function will notify the tool that a task has started executing
 \param timer     the tool-specific timer object that is associated with this task
 \param resource  the resource where the task is executing
 \sa @ref tasktimer_schedule_t
 \sa @ref tasktimer_yield_t
 \sa @ref tasktimer_resume_t
 \sa @ref tasktimer_stop_t
*/
typedef void tasktimer_start_t(tasktimer_timer_t, tasktimer_execution_space_p);

/**
 \brief Yield a task

 The yield function will notify the tool that a task has yielded
 \param timer     the tool-specific timer object that is associated with this task
 \sa @ref tasktimer_schedule_t
 \sa @ref tasktimer_start_t
 \sa @ref tasktimer_resume_t
 \sa @ref tasktimer_stop_t
*/
typedef void tasktimer_yield_t(tasktimer_timer_t);

/**
 \brief Resume a task

 The resume function will notify the tool that a task has resumed
 \param timer     the tool-specific timer object that is associated with this task
 \sa @ref tasktimer_schedule_t
 \sa @ref tasktimer_start_t
 \sa @ref tasktimer_yield_t
 \sa @ref tasktimer_stop_t
*/
typedef void tasktimer_resume_t(tasktimer_timer_t, tasktimer_execution_space_p);

/**
 \brief Stop a task

 The stop function will notify the tool that a task has stopped.
 \param timer     the tool-specific timer object that is associated with this task
 \sa @ref tasktimer_schedule_t
 \sa @ref tasktimer_start_t
 \sa @ref tasktimer_yield_t
 \sa @ref tasktimer_resume_t
*/
typedef void tasktimer_stop_t(tasktimer_timer_t);

/**
 \brief Destroy a task

 The destroy function will notify the tool that the task has been destroyed
 and that the tool should destroy the timer object.
 \param timer the timer to be destroyed
 \sa @ref tasktimer_create_t
*/
typedef void tasktimer_destroy_t(tasktimer_timer_t);

/**
 \brief Add parents to a task

 The add parents function will notify the tool that a task has had parents
 added to it that were not known at creation time.
 \param timer   the timer object
 \param parents GUIDs of the additional parents
 \param count   the number of parents being added
 \sa @ref tasktimer_add_children_t
*/
typedef void tasktimer_add_parents_t(tasktimer_timer_t, const tasktimer_guid_t*,
    const uint64_t);

/**
 \brief Add children to a task

 The add children function will notify the tool that a task has had children
 added to it that were not known at the creation time of the children.
 \param timer    the timer object
 \param children GUIDs of the additional parents
 \param count    the number of parents being added
 \sa @ref tasktimer_add_parents_t
*/
typedef void tasktimer_add_children_t(tasktimer_timer_t, const tasktimer_guid_t*,
    const uint64_t);

/**
 \brief Transfer data start

 The data transfer start function will notify that a data transfer will begin
 between two execution spaces, either host-host, host-device, device-host, or
 device-device.
 \param guid          identifier of the data transfer
 \param src_resource  the type of resource initiating the transfer
 \param src_name      the name of the source object
 \param src_ptr       a pointer to the source location
 \param dest_resource the type of resource receiving the transfer
 \param dest_name     the name of the destination object
 \param dest_ptr      a pointer to the destination location
 \sa @ref tasktimer_data_transfer_stop_t
*/
typedef void tasktimer_data_transfer_start_t(tasktimer_guid_t,
    tasktimer_execution_space_p, const char*, const void*,
    tasktimer_execution_space_p, const char*, const void*);

/**
 \brief Transfer data stop

 The data transfer stop function will notify that a data transfer has ended
 between two execution spaces, either host-host, host-device, device-host, or
 device-device.
 \param guid          identifier of the data transfer (is this necessary?)
 \sa @ref tasktimer_data_transfer_start_t
*/
typedef void tasktimer_data_transfer_stop_t(tasktimer_guid_t);

typedef void tasktimer_command_start_t(const char*);
typedef void tasktimer_command_stop_t();

#ifdef COMPILE_PLUGIN
#define PLUGIN_INTERFACE __attribute__((visibility("default")))
#else
#define PLUGIN_INTERFACE
#endif

/* Function declarations:
   the plugin loader will look for these, and
   the tool should implement them. */
#ifdef __cplusplus
extern "C" {
#endif
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
PLUGIN_INTERFACE tasktimer_data_transfer_start_t tasktimer_data_transfer_start;
PLUGIN_INTERFACE tasktimer_data_transfer_stop_t tasktimer_data_transfer_stop;
PLUGIN_INTERFACE tasktimer_command_start_t tasktimer_command_start;
PLUGIN_INTERFACE tasktimer_command_stop_t tasktimer_command_stop;
#ifdef __cplusplus
}
#endif

/*
 * Copyright (c) 2014-2021 Kevin Huck
 * Copyright (c) 2014-2021 University of Oregon
 *
 * Distributed under the BSD 2-Clause Software License. (See accompanying
 * file LICENSE)
 */

#include <stdio.h>
#include "tasktimer.h"
#include "plugin.h"

tasktimer_initialize_t* tasktimer_initialize_func = NULL;
tasktimer_finalize_t* tasktimer_finalize_func = NULL;
tasktimer_create_t* tasktimer_create_func = NULL;
tasktimer_schedule_t* tasktimer_schedule_func = NULL;
tasktimer_start_t* tasktimer_start_func = NULL;
tasktimer_yield_t* tasktimer_yield_func = NULL;
tasktimer_resume_t* tasktimer_resume_func = NULL;
tasktimer_stop_t* tasktimer_stop_func = NULL;
tasktimer_destroy_t* tasktimer_destroy_func = NULL;
tasktimer_add_parents_t* tasktimer_add_parents_func = NULL;
tasktimer_add_children_t* tasktimer_add_children_func = NULL;
tasktimer_data_transfer_start_t* tasktimer_data_transfer_start_func = NULL;
tasktimer_data_transfer_stop_t* tasktimer_data_transfer_stop_func = NULL;
tasktimer_command_start_t* tasktimer_command_start_func = NULL;
tasktimer_command_stop_t* tasktimer_command_stop_func = NULL;

/* After we're confident things are working, remove this. */
#define ECHO printf("in %s\n", __func__);

/* Look for a tool library, and assign the functions that it has implemented. */
void tasktimer_initialize(void) {
    ECHO
    tasktimer_initialize_func = (tasktimer_initialize_t*)(GET_SYMBOL(tasktimer_initialize_impl));
    if (tasktimer_initialize_func == NULL) {
        return;
    }
    /* Call the tool initializer function */
    tasktimer_initialize_func();
    /* Assign the other function pointers */
    tasktimer_finalize_func = (tasktimer_finalize_t*)(GET_SYMBOL(tasktimer_finalize_impl));
    tasktimer_create_func = (tasktimer_create_t*)(GET_SYMBOL(tasktimer_create_impl));
    tasktimer_schedule_func = (tasktimer_schedule_t*)(GET_SYMBOL(tasktimer_schedule_impl));
    tasktimer_start_func = (tasktimer_start_t*)(GET_SYMBOL(tasktimer_start_impl));
    tasktimer_yield_func = (tasktimer_yield_t*)(GET_SYMBOL(tasktimer_yield_impl));
    tasktimer_resume_func = (tasktimer_resume_t*)(GET_SYMBOL(tasktimer_resume_impl));
    tasktimer_stop_func = (tasktimer_stop_t*)(GET_SYMBOL(tasktimer_stop_impl));
    tasktimer_destroy_func = (tasktimer_destroy_t*)(GET_SYMBOL(tasktimer_destroy_impl));
    tasktimer_add_parents_func = (tasktimer_add_parents_t*)(GET_SYMBOL(tasktimer_add_parents_impl));
    tasktimer_add_children_func = (tasktimer_add_children_t*)(GET_SYMBOL(tasktimer_add_children_impl));
    tasktimer_data_transfer_start_func = (tasktimer_data_transfer_start_t*)(GET_SYMBOL(tasktimer_data_transfer_start_impl));
    tasktimer_data_transfer_stop_func = (tasktimer_data_transfer_stop_t*)(GET_SYMBOL(tasktimer_data_transfer_stop_impl));
    tasktimer_command_start_func = (tasktimer_command_start_t*)(GET_SYMBOL(tasktimer_command_start_impl));
    tasktimer_command_stop_func = (tasktimer_command_stop_t*)(GET_SYMBOL(tasktimer_command_stop_impl));
}

/* Finalize the tool */
void tasktimer_finalize(void) {
    ECHO
    if (tasktimer_finalize_func != NULL) {
        tasktimer_finalize_func();
    }
    return;
}

/* Notify the tool that a task has been created. */
tasktimer_timer_t tasktimer_create(const tasktimer_function_pointer_t address,
    const char* name, const tasktimer_guid_t guid,
    const tasktimer_guid_t* parents, const uint64_t parent_count) {
    ECHO
    tasktimer_timer_t timer = NULL;
    if (tasktimer_create_func != NULL) {
        timer = tasktimer_create_func(address, name, guid, parents, parent_count);
    }
    return timer;
}

/* Notify the tool that a task has been scheduled. */
void tasktimer_schedule(tasktimer_timer_t timer,
    tasktimer_argument_value_p arguments, uint64_t argument_count) {
    ECHO
    if (tasktimer_schedule_func != NULL) {
        tasktimer_schedule_func(timer, arguments, argument_count);
    }
    return;
}

/* Notify the tool that a task has been started. */
void tasktimer_start(tasktimer_timer_t timer, tasktimer_execution_space_p resource) {
    ECHO
    if (tasktimer_start_func != NULL) {
        tasktimer_start_func(timer, resource);
    }
    return;
}

/* Notify the tool that a task has been yielded. */
void tasktimer_yield(tasktimer_timer_t timer) {
    ECHO
    if (tasktimer_yield_func != NULL) {
        tasktimer_yield_func(timer);
    }
    return;
}

/* Notify the tool that a task has been resumed. */
void tasktimer_resume(tasktimer_timer_t timer, tasktimer_execution_space_p resource) {
    ECHO
    if (tasktimer_resume_func != NULL) {
        tasktimer_resume_func(timer, resource);
    }
    return;
}

/* Notify the tool that a task has been stopped. */
void tasktimer_stop(tasktimer_timer_t timer) {
    ECHO
    if (tasktimer_stop_func != NULL) {
        tasktimer_stop_func(timer);
    }
    return;
}

/* Notify the tool that a task has been destoyed. */
void tasktimer_destroy(tasktimer_timer_t timer) {
    ECHO
    if (tasktimer_destroy_func != NULL) {
        tasktimer_destroy_func(timer);
    }
    return;
}

/* Notify the tool that a task has had more parents assigned to it. */
void tasktimer_add_parents(tasktimer_timer_t timer, const tasktimer_guid_t* parents,
    const uint64_t parent_count) {
    ECHO
    if (tasktimer_add_parents_func != NULL) {
        tasktimer_add_parents_func(timer, parents, parent_count);
    }
    return;
}

/* Notify the tool that a task has had more children assigned to it. */
void tasktimer_add_children(tasktimer_timer_t timer, const tasktimer_guid_t* children,
    const uint64_t child_count) {
    ECHO
    if (tasktimer_add_children_func != NULL) {
        tasktimer_add_children_func(timer, children, child_count);
    }
    return;
}

void tasktimer_data_transfer_start(tasktimer_guid_t guid,
    tasktimer_execution_space_p source_resource_type, const char* source_name, const void* source_ptr,
    tasktimer_execution_space_p dest_resource_type, const char* dest_name, const void* dest_ptr) {
    ECHO
    if (tasktimer_data_transfer_start_func != NULL) {
        tasktimer_data_transfer_start_func(guid, source_resource_type, source_name, source_ptr, dest_resource_type, dest_name, dest_ptr);
    }
    return;
}

void tasktimer_data_transfer_stop(tasktimer_guid_t guid) {
    ECHO
    if (tasktimer_data_transfer_stop_func != NULL) {
        tasktimer_data_transfer_stop_func(guid);
    }
    return;
}

void tasktimer_command_start(const char* type_name) {
    ECHO
    if (tasktimer_command_start_func != NULL) {
        tasktimer_command_start_func(type_name);
    }
    return;
}

void tasktimer_command_stop(void) {
    ECHO
    if (tasktimer_command_stop_func != NULL) {
        tasktimer_command_stop_func();
    }
    return;
}



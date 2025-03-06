/*
 * Copyright (c) 2025 Kevin Huck
 * Copyright (c) 2025 University of Oregon
 *
 * Distributed under the BSD 2-Clause Software License. (See accompanying
 * file LICENSE)
 *
 * Summary: This file is an example tool created to demonstrate the taskstubs
 * interface. This example implements the taskstubs calls as ROCTX API calls.
 */

#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "tool_api.h"
#include <roctracer/roctx.h>
#include <map>
#include <string>
#include <stack>
#include <iostream>
#if defined(__GNUC__)
#include <cxxabi.h>
#endif

void doMarker(std::string name) {
    //std::cout << name << std::endl;
    roctxMarkA(name.c_str());
}

void doMarker(const std::string& name, const std::string& name2) {
    std::string tmpstr(name);
    tmpstr += std::string(":") + std::string(name2);
    doMarker(tmpstr.c_str());
}

std::string demangle(const std::string& timer_name) {
    std::string demangled = std::string(timer_name);
#if defined(__GNUC__)
    int     status;
    char *realname = abi::__cxa_demangle(timer_name.c_str(), 0, 0, &status);
    if (status == 0 && realname != nullptr) {
        demangled = std::string(realname);
        free(realname);
    } else {
        switch (status) {
            case 0:
                fprintf(stderr, "The demangling operation succeeded, but realname is NULL\n");
                break;
            case -1:
                fprintf(stderr, "The demangling operation failed:");
                fprintf(stderr, " A memory allocation failiure occurred.\n");
                break;
            case -2:
                // Commenting out, this is a silly message.
                fprintf(stderr, "The demangling operation failed:");
                fprintf(stderr, " '%s' is not a valid", timer_name.c_str());
                fprintf(stderr, " name under the C++ ABI mangling rules.\n");
                break;
            case -3:
                fprintf(stderr, "The demangling operation failed: One of the");
                fprintf(stderr, " arguments is invalid.\n");
                break;
            default:
                fprintf(stderr, "The demangling operation failed: Unknown error.\n");
                break;
        }
    }
#endif
    return demangled;
}

class roctxTimer {
    private:
        std::string _name;
        uint64_t _guid;
        roctx_range_id_t _rangeId;
        bool doDestroy;
    public:
        roctxTimer(std::string name, uint64_t id, bool doCreate=false) :
            _name(name), _guid(id), _rangeId(0), doDestroy(doCreate) {
            if (doCreate) {
                doMarker("Timer Create", _name);
            }
        }
        ~roctxTimer(void) {
            if (doDestroy) {
                doMarker("Timer Destroy");
            }
        }
        void schedule() {
            doMarker("Timer Schedule");
        }
        void start() {
            //std::cout << "Start " << _name << std::endl;
            if (_rangeId == 0) {
                _rangeId = roctxRangeStartA(_name.c_str());
            }
        }
        void yield() {
            if (_rangeId > 0) {
                roctxRangeStop(_rangeId);
                _rangeId = 0;
            }
        }
        void resume() {
            if (_rangeId == 0) {
                _rangeId = roctxRangeStartA(_name.c_str());
            }
        }
        void stop() {
            //std::cout << "Stop " << _name << std::endl;
            if (_rangeId > 0) {
                roctxRangeStop(_rangeId);
                _rangeId = 0;
            }
        }
};

extern "C" {
void tasktimer_initialize_impl(void) {
    printf("Initializing ROCTX tool\n");
    doMarker("TaskStubs Initialize");
}

void tasktimer_finalize_impl(void) {
    printf("Finalizing ROCTX tool\n");
    doMarker("TaskStubs Finalize");
}

tasktimer_timer_t tasktimer_create_impl(const tasktimer_function_pointer_t address,
    const char* name, const uint64_t id, const uint64_t* parents,
    const uint64_t count) {
    std::string tmpstr;
    Dl_info info;
    if (name == nullptr) {
        int rc = dladdr((const void *)address, &info);
        if (rc == 0 || info.dli_sname == nullptr) { 
        } else {
            //node->info.filename = strdup(info.dli_fname);
            tmpstr = info.dli_sname;
            tmpstr = demangle(tmpstr);
            if (info.dli_fname != nullptr) {
                tmpstr += std::string(" [") + info.dli_fname + std::string("]");
            }
        }
    } else {
        tmpstr = name;
    }
    auto* timer = new roctxTimer(tmpstr, id, true);
    return timer;
}

void tasktimer_destroy_impl(tasktimer_timer_t timer) {
    auto* t = static_cast<roctxTimer*>(timer);
    delete t;
}

void tasktimer_schedule_impl(tasktimer_timer_t in_timer,
    tasktimer_argument_value_t* arguments, uint64_t argument_count) {
    auto* t = static_cast<roctxTimer*>(in_timer);
    t->schedule();
}

void tasktimer_start_impl(tasktimer_timer_t in_timer, tasktimer_execution_space_p resource) {
    auto* t = static_cast<roctxTimer*>(in_timer);
    t->start();
}

void tasktimer_yield_impl(tasktimer_timer_t in_timer) {
    auto* t = static_cast<roctxTimer*>(in_timer);
    t->yield();
}

void tasktimer_resume_impl(tasktimer_timer_t in_timer, tasktimer_execution_space_p resource) {
    auto* t = static_cast<roctxTimer*>(in_timer);
    t->resume();
}

void tasktimer_stop_impl(tasktimer_timer_t in_timer) {
    auto* t = static_cast<roctxTimer*>(in_timer);
    t->stop();
}

void tasktimer_add_parents_impl(tasktimer_timer_t in_timer,
    const uint64_t* parents, const uint64_t count) {
    doMarker("Timer Add Parents");
}

void tasktimer_add_children_impl(tasktimer_timer_t in_timer,
    const uint64_t* children, const uint64_t count) {
    doMarker("Timer Add Children");
}

std::stack<roctxTimer*> timerStack;

void tasktimer_data_transfer_start_impl(tasktimer_guid_t guid,
    tasktimer_execution_space_p source_type, const char* source_name, const void* source_ptr,
    tasktimer_execution_space_p dest_type, const char* dest_name, const void* dest_ptr) {
    const auto getspace = [](tasktimer_execution_space_p& space) {
        std::string ss;
        ss += std::string((space->type == TASKTIMER_DEVICE_CPU ? "CPU " : "GPU "));
        ss += std::to_string(space->device_id);
        ss += std::string(",");
        ss += std::to_string(space->instance_id);
        return ss;
    };  
    std::string tmpstr{"Data Transfer "};
    tmpstr += getspace(source_type);
    tmpstr += std::string(" -> ");
    tmpstr += getspace(dest_type);
    roctxTimer* timer = new roctxTimer(tmpstr, guid);
    timerStack.push(timer);
    timer->start();
}

void tasktimer_data_transfer_stop_impl(tasktimer_guid_t guid) {
    roctxTimer* timer = timerStack.top();
    timerStack.pop();
    timer->stop();
}

void tasktimer_command_start_impl(const char* type_name) {
    roctxTimer* timer = new roctxTimer(type_name, 0);
    timerStack.push(timer);
    timer->start();
}

void tasktimer_command_stop_impl(void) {
    roctxTimer* timer = timerStack.top();
    timerStack.pop();
    timer->stop();
}

} // extern "C"


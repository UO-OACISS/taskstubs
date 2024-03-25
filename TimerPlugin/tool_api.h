#pragma once

#include <stdint.h>

#ifdef COMPILE_PLUGIN
#define PLUGIN_INTERFACE __attribute__((visibility("default")))
#else
#define PLUGIN_INTERFACE
#endif

#ifdef __cplusplus

#include <memory>
#include <functional>
#include <set>

extern "C" {
PLUGIN_INTERFACE std::shared_ptr<void> tasktimer_impl_create_string(const std::string,
    const uint64_t, const std::set<uint64_t>&);
PLUGIN_INTERFACE std::shared_ptr<void> tasktimer_impl_create_address(const uintptr_t,
    const uint64_t, const std::set<uint64_t>&);
PLUGIN_INTERFACE void tasktimer_impl_schedule(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_impl_start(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_impl_yield(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_impl_resume(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_impl_stop(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_impl_add_parents(std::shared_ptr<void>, const std::set<uint64_t>&);
PLUGIN_INTERFACE void tasktimer_impl_add_children(std::shared_ptr<void>, const std::set<uint64_t>&);
}

#else

PLUGIN_INTERFACE void* tasktimer_impl_create_string(const char*,
    const uint64_t, const uint64_t*, const uint64_t);
PLUGIN_INTERFACE void* tasktimer_impl_create_address(const uintptr_t,
    const uint64_t, const uint64_t*, const uint64_t);
PLUGIN_INTERFACE void tasktimer_impl_schedule(void*);
PLUGIN_INTERFACE void tasktimer_impl_start(void*);
PLUGIN_INTERFACE void tasktimer_impl_yield(void*);
PLUGIN_INTERFACE void tasktimer_impl_resume(void*);
PLUGIN_INTERFACE void tasktimer_impl_stop(void*);
PLUGIN_INTERFACE void tasktimer_impl_add_parents(void*, const uint64_t*, const uint64_t);
PLUGIN_INTERFACE void tasktimer_impl_add_children(void*, const uint64_t*, const uint64_t);

#endif


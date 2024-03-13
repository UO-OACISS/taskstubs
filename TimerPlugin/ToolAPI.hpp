#pragma once

#ifdef COMPILE_PLUGIN
#define PLUGIN_INTERFACE __attribute__((visibility("default")))
#else
#define PLUGIN_INTERFACE
#endif

#include <memory>
#include <functional>
#include <set>

extern "C" {
PLUGIN_INTERFACE std::shared_ptr<void> tasktimer_create_string(const std::string,
    const uint64_t, const std::set<uint64_t>&);
PLUGIN_INTERFACE std::shared_ptr<void> tasktimer_create_address(const uintptr_t,
    const uint64_t, const std::set<uint64_t>&);
PLUGIN_INTERFACE void tasktimer_schedule(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_start(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_yield(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_resume(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_stop(std::shared_ptr<void>);
PLUGIN_INTERFACE void tasktimer_add_parents(std::shared_ptr<void>, const std::set<uint64_t>&);
PLUGIN_INTERFACE void tasktimer_add_children(std::shared_ptr<void>, const std::set<uint64_t>&);
}


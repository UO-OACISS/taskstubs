#include "Plugin.hpp"
#include "tool_api.h"
#include <dlfcn.h>
#include <string>
#include <iostream>
#include <set>

#ifndef TASKTIMER_PLUGIN_ENVIRONMENT_VARIABLE_NAME
#define TASKTIMER_PLUGIN_ENVIRONMENT_VARIABLE_NAME "TASKTIMER_EXTERNAL_TOOL_LIBRARY"
#endif

Plugin::Plugin() : m_libraryHandle{getLibrary()} { }

std::shared_ptr<void> Plugin::getLibrary(void) {
    /* If, for some reason, we haven't preloaded the libXXX.so
     * library, Go looking for the library, and then the symbol.
     * This assumes that the LD_LIBRARY_PATH will include the
     * path to the library. */

    /* Check for the environment variable */
    const char * libname = getenv(TASKTIMER_PLUGIN_ENVIRONMENT_VARIABLE_NAME);
    /* If not set, return */
    if (libname == nullptr) {
        return nullptr;
    }
    /* Check to see if we've already loaded it */
    auto handle = dlopen(libname, RTLD_NOLOAD | RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
        /* Library hasn't been loaded, so load it */
        handle = dlopen(libname, RTLD_NOW | RTLD_LOCAL);
    }
    if (handle == nullptr) {
        std::cerr << "Unable to load library " << libname << std::endl;
        throw std::runtime_error(dlerror());
    }
    return std::shared_ptr<void>(handle, dlclose);
}

void* Plugin::getSymbol(const char * symbol) {
    // Do we have a library to search?
    if (m_libraryHandle == nullptr) {
        return nullptr;
    }
    // Do we have the symbol already?
    auto func = dlsym(RTLD_DEFAULT, symbol);
    if (func != nullptr) {
        return func;
    }
    func = dlsym(m_libraryHandle.get(), symbol);
    if (func == nullptr) {
        std::cerr << "Unable to load symbol " << symbol << std::endl;
        return nullptr;
    }
    return func;
}

#define GET_FUNCTION_POINTER(name) reinterpret_cast<decltype(name)*>(getSymbol(#name))

std::shared_ptr<void> Plugin::create(const std::string name, const uint64_t id,
    const std::set<uint64_t>& parents) {
    static auto func{GET_FUNCTION_POINTER(tasktimer_impl_create_string)};
    if (func == nullptr) return nullptr;
    return func(name, id, parents);
}

std::shared_ptr<void> Plugin::create(const uintptr_t address, const uint64_t id,
    const std::set<uint64_t>& parents) {
    static auto func{GET_FUNCTION_POINTER(tasktimer_impl_create_address)};
    if (func == nullptr) return nullptr;
    return func(address, id, parents);
}

void Plugin::schedule(std::shared_ptr<void> externalTimer) {
    static auto func{GET_FUNCTION_POINTER(tasktimer_impl_schedule)};
    if (func == nullptr) return;
    func(externalTimer);
}

void Plugin::start(std::shared_ptr<void> externalTimer) {
    static auto func{GET_FUNCTION_POINTER(tasktimer_impl_start)};
    if (func == nullptr) return;
    func(externalTimer);
}

void Plugin::yield(std::shared_ptr<void> externalTimer) {
    static auto func{GET_FUNCTION_POINTER(tasktimer_impl_yield)};
    if (func == nullptr) return;
    func(externalTimer);
}

void Plugin::resume(std::shared_ptr<void> externalTimer) {
    static auto func{GET_FUNCTION_POINTER(tasktimer_impl_resume)};
    if (func == nullptr) return;
    func(externalTimer);
}

void Plugin::stop(std::shared_ptr<void> externalTimer) {
    static auto func{GET_FUNCTION_POINTER(tasktimer_impl_stop)};
    if (func == nullptr) return;
    func(externalTimer);
}

void Plugin::addParents(std::shared_ptr<void> externalTimer,
    const std::set<uint64_t>& parents) {
    static auto func{GET_FUNCTION_POINTER(tasktimer_impl_add_parents)};
    if (func == nullptr) return;
    func(externalTimer, parents);
}

void Plugin::addChildren(std::shared_ptr<void> externalTimer,
    const std::set<uint64_t>& children) {
    static auto func{GET_FUNCTION_POINTER(tasktimer_impl_add_children)};
    if (func == nullptr) return;
    func(externalTimer, children);
}


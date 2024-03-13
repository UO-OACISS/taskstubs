#pragma once
#include <memory>
#include <functional>
#include <set>
#include "Plugin.hpp"

class TaskTimer {
        std::shared_ptr<void> externalTimer;
    public:
        /* Task lifetime */
        TaskTimer(const std::string name, const uint64_t id,
            const std::set<uint64_t>& parents = {}) :
            externalTimer{Plugin::instance().create(name, id, parents)} { }
        TaskTimer(const uintptr_t address, const uint64_t id,
            const std::set<uint64_t>& parents = {}) :
            externalTimer{Plugin::instance().create(address, id, parents)} { }
        void schedule(void) {
            Plugin::instance().schedule(externalTimer);
        }
        void start(void) {
            Plugin::instance().start(externalTimer);
        }
        void yield(void) {
            Plugin::instance().yield(externalTimer);
        }
        void resume(void) {
            Plugin::instance().resume(externalTimer);
        }
        void stop(void) {
            Plugin::instance().stop(externalTimer);
        }
        void addParents(const std::set<uint64_t>& parents) {
            Plugin::instance().addParents(externalTimer, parents);
        }
        void addChildren(const std::set<uint64_t>& children) {
            Plugin::instance().addChildren(externalTimer, children);
        }
        ~TaskTimer(void) {};
};
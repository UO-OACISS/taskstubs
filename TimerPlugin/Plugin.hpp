#pragma once
#include <memory>
#include <functional>
#include <set>

class Plugin {
    public:
        Plugin(const Plugin&) = delete;
        Plugin(Plugin&&) = delete;
        Plugin& operator=(const Plugin&) = delete;
        Plugin& operator=(Plugin&&) = delete;
        static Plugin& instance(void) {
            static Plugin _inst;
            return _inst;
        };
        std::shared_ptr<void> create(const std::string name, const uint64_t id,
            const std::set<uint64_t>& parents);
        std::shared_ptr<void> create(const uintptr_t address, const uint64_t id,
            const std::set<uint64_t>& parents);
        void schedule(std::shared_ptr<void> taskTimer);
        void start(std::shared_ptr<void> taskTimer);
        void yield(std::shared_ptr<void> taskTimer);
        void resume(std::shared_ptr<void> taskTimer);
        void stop(std::shared_ptr<void> taskTimer);
        void addParents(std::shared_ptr<void> taskTimer,
            const std::set<uint64_t>& parents);
        void addChildren(std::shared_ptr<void> taskTimer,
            const std::set<uint64_t>& children);
    private:
        Plugin(void);
        std::shared_ptr<void> getLibrary(void);
        void* getSymbol(const char * symbol);
        std::shared_ptr<void> m_libraryHandle;
};

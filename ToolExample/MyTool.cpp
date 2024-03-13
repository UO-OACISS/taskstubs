#include <iostream>
#include <functional>
#include <set>
#include <map>
#include <sstream>

#include "TimerPlugin/TaskTimer.hpp"

class MyTimer {
    public:
        std::string mName;
        uint64_t mID;
        bool hasName;
        uintptr_t mAddress;
        std::set<uint64_t> mParents;
        std::set<uint64_t> mChildren;
        std::string label() {
            if (hasName) return mName;
            return std::to_string(mAddress);
        }
        std::string printSet(const std::set<uint64_t>& inSet) {
            std::stringstream ss;
            ss << "[";
            bool first{true};
            for (auto const& e : inSet) {
                if (!first) ss << ",";
                ss << e;
                first = false;
            }
            ss << "]";
            return ss.str();
        }
        MyTimer(const std::string name, const uint64_t id,
            const std::set<uint64_t> parents) :
            mName(name), mAddress(0), mID(id), hasName(true), mParents(parents) {
            std::cout << "Created timer " << label()
                      << " with ID " << mID
                      << " and parents " << printSet(parents)
                      << std::endl;
            knownTimers[id] = this;
            for (auto& e : parents) { knownTimers[e]->addChild(id); }
        }
        MyTimer(const uintptr_t address, const uint64_t id,
            const std::set<uint64_t> parents) :
            mName(""), mAddress(address), mID(id), hasName(false), mParents(parents) {
            std::cout << "Created timer " << label()
                      << " with ID " << mID
                      << " and parents " << printSet(parents)
                      << std::endl;
            knownTimers[id] = this;
            for (auto& e : parents) { knownTimers[e]->addChild(id); }
        }
        ~MyTimer(void) {
            std::cout << "Destroying timer " << label()
                      << " with ID " << mID
                      << " and parents " << printSet(mParents)
                      << " and children " << printSet(mChildren)
                      << std::endl;
        }
        void schedule(void) {
            std::cout << "Scheduling timer " << label()
                      << " with ID " << mID << std::endl;
        }
        void start(void) {
            std::cout << "Starting timer " << label()
                      << " with ID " << mID << std::endl;
        }
        void yield(void) {
            std::cout << "Yielding timer " << label()
                      << " with ID " << mID << std::endl;
        }
        void resume(void) {
            std::cout << "Resuming timer " << label()
                      << " with ID " << mID << std::endl;
        }
        void stop(void) {
            std::cout << "Stopping timer " << label()
                      << " with ID " << mID << std::endl;
        }
        void addParents(const std::set<uint64_t>& parents) {
            mParents.merge(const_cast<std::set<uint64_t>&>(parents));
            std::cout << "Timer " << label()
                      << " adding parents " << printSet(parents)
                      << std::endl;
        }
        void addChildren(const std::set<uint64_t>& children) {
            mChildren.merge(const_cast<std::set<uint64_t>&>(children));
            std::cout << "Timer " << label()
                      << " adding children " << printSet(children)
                      << std::endl;
        }
    private:
        static std::map<uint64_t,MyTimer*> knownTimers;
        void addChild(const uint64_t c) {
            mChildren.insert(c);
        }
};

std::map<uint64_t,MyTimer*> MyTimer::knownTimers;

extern "C" {
std::shared_ptr<void> tasktimer_create_string(const std::string name, const uint64_t id,
    const std::set<uint64_t>& parents) {
    std::shared_ptr<MyTimer> tmp = std::make_shared<MyTimer>(name,id,parents);
    return tmp;
}

std::shared_ptr<void> tasktimer_create_address(const uintptr_t address, const uint64_t id,
    const std::set<uint64_t>& parents) {
    std::shared_ptr<MyTimer> tmp = std::make_shared<MyTimer>(address,id,parents);
    return tmp;
}

void tasktimer_schedule(const std::shared_ptr<void> taskTimer) {
    auto tmp = std::static_pointer_cast<MyTimer>(taskTimer);
    tmp->schedule();
}

void tasktimer_start(const std::shared_ptr<void> taskTimer) {
    auto tmp = std::static_pointer_cast<MyTimer>(taskTimer);
    tmp->start();
}

void tasktimer_yield(const std::shared_ptr<void> taskTimer) {
    auto tmp = std::static_pointer_cast<MyTimer>(taskTimer);
    tmp->yield();
}

void tasktimer_resume(const std::shared_ptr<void> taskTimer) {
    auto tmp = std::static_pointer_cast<MyTimer>(taskTimer);
    tmp->resume();
}

void tasktimer_stop(const std::shared_ptr<void> taskTimer) {
    auto tmp = std::static_pointer_cast<MyTimer>(taskTimer);
    tmp->stop();
}

void tasktimer_add_parents(const std::shared_ptr<void> taskTimer,
    const std::set<uint64_t>& parents) {
    auto tmp = std::static_pointer_cast<MyTimer>(taskTimer);
    tmp->addParents(parents);
}

void tasktimer_add_children(const std::shared_ptr<void> taskTimer,
    const std::set<uint64_t>& children) {
    auto tmp = std::static_pointer_cast<MyTimer>(taskTimer);
    tmp->addChildren(children);
}

}


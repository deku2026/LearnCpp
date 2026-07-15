// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : double_checked_locking
// Topic id : part6/f/section05/double_checked_locking
//
// Covers: double-checked locking with atomics (safe pattern)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <memory>
#include <mutex>

namespace {

class Singleton {
    static std::atomic<Singleton*> instance;
    static std::mutex m;
    int value_ = 42;
    Singleton() = default;

public:
    static Singleton& get() {
        Singleton* p = instance.load(std::memory_order_acquire);
        if (!p) {
            std::lock_guard<std::mutex> lock(m);
            p = instance.load(std::memory_order_relaxed);
            if (!p) {
                p = new Singleton();
                instance.store(p, std::memory_order_release);
            }
        }
        return *p;
    }
    int value() const { return value_; }
};
std::atomic<Singleton*> Singleton::instance{nullptr};
std::mutex Singleton::m;

void demo_basics() {
    assert(Singleton::get().value() == 42);
}

void demo_intermediate() {
    assert(&Singleton::get() == &Singleton::get());
}

void demo_expert() {
    // Prefer function-local static (Meyers) unless you need lazy dynamic lifetime control.
    assert(Singleton::get().value() == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/double_checked_locking", run>;

}  // namespace

// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section02_raii
// Item     : raii_exception_safety_pairing
// Topic id : part2/stage04/section02/raii_exception_safety_pairing
//
// Covers: RAII unlocks on throw/early return; lock_guard pairing

#include "learn/topic_registry.hpp"

#include <mutex>
#include <stdexcept>

namespace {

std::mutex g_mtx;
int g_locked = 0;

struct CountingLock {
    explicit CountingLock(std::mutex& m) : m_(m) {
        m_.lock();
        ++g_locked;
    }
    ~CountingLock() {
        --g_locked;
        m_.unlock();
    }
    CountingLock(const CountingLock&) = delete;
    CountingLock& operator=(const CountingLock&) = delete;

private:
    std::mutex& m_;
};

bool everything_ok(bool ok) {
    return ok;
}

void good_path(bool ok) {
    CountingLock lk(g_mtx);
    if (!everything_ok(ok)) {
        return;
    }
    LEARN_CHECK(g_locked == 1);
}

void good_throw() {
    CountingLock lk(g_mtx);
    throw std::runtime_error("demo");
}

void demo_basics() {
    g_locked = 0;
    good_path(true);
    LEARN_CHECK(g_locked == 0);
    good_path(false);
    LEARN_CHECK(g_locked == 0);
}

void demo_intermediate() {
    g_locked = 0;
    try {
        good_throw();
    } catch (const std::runtime_error&) {
        // resource released on unwind
    }
    LEARN_CHECK(g_locked == 0);
}

void demo_expert() {
    g_locked = 0;
    {
        std::lock_guard<std::mutex> lk(g_mtx);
        LEARN_CHECK(g_locked == 0);  // lock_guard does not use our counter
        // still exception-safe pairing for the mutex itself
    }
    // Nested RAII: outer + inner both release on scope exit.
    {
        CountingLock outer(g_mtx);
        LEARN_CHECK(g_locked == 1);
        {
            // Cannot re-lock same non-recursive mutex; use a second mutex.
            static std::mutex other;
            CountingLock inner(other);
            LEARN_CHECK(g_locked == 2);
        }
        LEARN_CHECK(g_locked == 1);
    }
    LEARN_CHECK(g_locked == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section02/raii_exception_safety_pairing", run>;

}  // namespace

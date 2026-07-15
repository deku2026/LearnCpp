// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section03_const_constexpr_volatile
// Item     : volatile_is_not_thread_sync
// Topic id : part2/stage02/section03/volatile_is_not_thread_sync
//
// Covers: volatile semantics for MMIO-like access; not a mutex/atomic substitute

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <type_traits>

namespace {

void demo_basics() {
    volatile int sensor = 0;
    sensor = 10;
    int snapshot = sensor;
    assert(snapshot == 10);

    sensor = sensor + 1;
    assert(sensor == 11);
}

void demo_intermediate() {
    // volatile prevents some optimizations on that object, but does not order threads.
    volatile int flag = 0;
    flag = 1;
    assert(flag == 1);

    // For inter-thread communication, use std::atomic (or mutexes).
    std::atomic<int> ready{0};
    ready.store(1, std::memory_order_release);
    assert(ready.load(std::memory_order_acquire) == 1);

    static_assert(std::is_volatile_v<volatile int>);
    static_assert(!std::is_volatile_v<int>);
}

void demo_expert() {
    // const volatile: read-only view of a location that may change externally.
    int storage = 5;
    const volatile int* view = &storage;
    assert(*view == 5);
    storage = 6;
    assert(*view == 6);

    // Atomic is the correct tool for shared flags between threads.
    std::atomic_bool done{false};
    done = true;
    assert(done.load());

    // Qualifier combination demos
    using CV = const volatile int;
    static_assert(std::is_const_v<CV>);
    static_assert(std::is_volatile_v<CV>);

    volatile int counter = 0;
    counter = counter + 1;
    assert(counter == 1);

    // Summary asserted via types: volatile ≠ atomic
    static_assert(!std::is_same_v<volatile int, std::atomic<int>>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/volatile_is_not_thread_sync", run>;

}  // namespace

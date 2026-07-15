// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section03_atomic_helpers
// Item     : atomic_ref_cpp20
// Topic id : part6/f/section03/atomic_ref_cpp20
//
// Covers: std::atomic_ref for existing non-atomic objects (C++20)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
#if defined(__cpp_lib_atomic_ref) && __cpp_lib_atomic_ref >= 201806L
    alignas(std::atomic_ref<int>::required_alignment) int x = 0;
    std::atomic_ref<int> r(x);
    r.store(3);
    LEARN_CHECK(r.load() == 3);
    LEARN_CHECK(x == 3);
#else
    std::atomic<int> x{0};
    x.store(3);
    LEARN_CHECK(x.load() == 3);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_atomic_ref) && __cpp_lib_atomic_ref >= 201806L
    alignas(std::atomic_ref<int>::required_alignment) int x = 0;
    std::atomic_ref<int> r(x);
    auto inc = [&] {
        for (int i = 0; i < 500; ++i) r.fetch_add(1);
    };
    std::thread t1(inc), t2(inc);
    t1.join();
    t2.join();
    LEARN_CHECK(r.load() == 1000);
#else
    std::atomic<int> x{0};
    auto inc = [&] {
        for (int i = 0; i < 500; ++i) x.fetch_add(1);
    };
    std::thread t1(inc), t2(inc);
    t1.join();
    t2.join();
    LEARN_CHECK(x.load() == 1000);
#endif
}

void demo_expert() {
    // Lifetime of referenced object must outlive atomic_ref uses; alignment required.
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section03/atomic_ref_cpp20", run>;

}  // namespace

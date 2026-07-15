// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : sanitizer_full_family_as_ub_detectors
// Topic id : part6/g/section05/sanitizer_full_family_as_ub_detectors
//
// Covers: ASan/UBSan/TSan as dynamic UB detectors

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>
#include <vector>

namespace {

void demo_basics() {
    // ASan: bounds/UAF; UBSan: overflow/null; TSan: data races — when built with sanitizers.
    std::vector<int> v{1, 2, 3};
    LEARN_CHECK(v.at(1) == 2);
}

void demo_intermediate() {
    std::atomic<int> c{0};
    std::thread t1([&] { c.fetch_add(1); });
    std::thread t2([&] { c.fetch_add(1); });
    t1.join();
    t2.join();
    LEARN_CHECK(c.load() == 2);
}

void demo_expert() {
    int* p = new int(1);
    LEARN_CHECK(*p == 1);
    delete p;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/sanitizer_full_family_as_ub_detectors", run>;

}  // namespace

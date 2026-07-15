// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : memory_order_per_hardware_x86_arm
// Topic id : part6/f/section01/memory_order_per_hardware_x86_arm
//
// Covers: hardware differences: x86 TSO vs weaker ARM (conceptual + portable atomics)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    // Portable code uses atomics; do not rely on x86 strong ordering alone.
    std::atomic<int> x{0};
    x.store(1, std::memory_order_release);
    LEARN_CHECK(x.load(std::memory_order_acquire) == 1);
}

void demo_intermediate() {
    std::atomic<int> flag{0};
    int data = 0;
    std::thread p([&] {
        data = 8;
        flag.store(1, std::memory_order_release);
    });
    std::thread c([&] {
        while (!flag.load(std::memory_order_acquire)) {
        }
        LEARN_CHECK(data == 8);
    });
    p.join();
    c.join();
}

void demo_expert() {
    // Teaching: on x86 many release/acquire lower to ordinary MOV; on ARM barriers matter.
    // Always write the intended memory_order, not what your CPU happens to do.
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/memory_order_per_hardware_x86_arm", run>;

}  // namespace

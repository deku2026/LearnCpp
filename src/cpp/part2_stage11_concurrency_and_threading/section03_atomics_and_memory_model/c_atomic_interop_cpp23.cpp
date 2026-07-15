// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : c_atomic_interop_cpp23
// Topic id : part2/stage11/section03/c_atomic_interop_cpp23
//
// Covers: C atomic interop / lock-free aliases

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cstdint>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<std::int32_t> x{0};
    x.store(3);
    LEARN_CHECK(x.load() == 3);
}

void demo_intermediate() {
    std::atomic<std::uint64_t> counter{0};
    std::thread t([&] { counter.fetch_add(10); });
    t.join();
    LEARN_CHECK(counter.load() == 10);
}

void demo_expert() {
#if defined(__cpp_lib_atomic_lock_free_type_aliases) && __cpp_lib_atomic_lock_free_type_aliases >= 201907L
    std::atomic_signed_lock_free s{0};
    s.store(1);
    LEARN_CHECK(s.load() == 1);
#else
    std::atomic<long> s{0};
    s.store(1);
    LEARN_CHECK(s.load() == 1);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/c_atomic_interop_cpp23", run>;

}  // namespace

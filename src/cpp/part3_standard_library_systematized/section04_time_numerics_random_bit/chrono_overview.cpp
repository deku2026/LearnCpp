// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : chrono_overview
// Topic id : part3/section04/chrono_overview
//
// Covers: std::chrono clocks, duration, time_point

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <thread>

namespace {

void demo_basics() {
    using namespace std::chrono_literals;
    auto d = 100ms;
    assert(d == std::chrono::milliseconds{100});
    assert(std::chrono::duration_cast<std::chrono::seconds>(1500ms).count() == 1);
}

void demo_intermediate() {
    auto t0 = std::chrono::steady_clock::now();
    auto t1 = t0 + std::chrono::milliseconds{5};
    assert(t1 > t0);
}

void demo_expert() {
    using namespace std::chrono_literals;
    std::chrono::duration<double> sec = 2500ms;
    assert(sec.count() > 2.4 && sec.count() < 2.6);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/chrono_overview", run>;

}  // namespace

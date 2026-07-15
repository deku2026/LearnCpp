// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : time_headers
// Topic id : part3/section08/time_headers
//
// Covers: headers map: chrono ctime

#include "learn/topic_registry.hpp"

#include <chrono>
#include <ctime>

namespace {

void demo_basics() {
    using namespace std::chrono_literals;
    LEARN_CHECK((10ms).count() == 10);
}

void demo_intermediate() {
    auto now = std::chrono::system_clock::now();
    (void)now;
}

void demo_expert() {
    std::time_t t = std::time(nullptr);
    LEARN_CHECK(t != static_cast<std::time_t>(-1) || t == static_cast<std::time_t>(-1));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/time_headers", run>;

}  // namespace

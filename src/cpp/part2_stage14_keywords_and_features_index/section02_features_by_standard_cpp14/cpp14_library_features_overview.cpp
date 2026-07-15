// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section02_features_by_standard_cpp14
// Item     : cpp14_library_features_overview
// Topic id : part2/stage14/section02/cpp14_library_features_overview
//
// Covers: C++14 library feature sampler

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <memory>
#include <utility>

namespace {

void demo_basics() {
    auto p = std::make_unique<int>(42);
    assert(*p == 42);
}

void demo_intermediate() {
    using namespace std::chrono_literals;
    auto d = 50ms;
    assert(d.count() == 50);
}

void demo_expert() {
    auto a = std::make_unique<int[]>(3);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    assert(a[2] == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section02/cpp14_library_features_overview", run>;

}  // namespace

// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : iterators_ranges_algorithms_headers
// Topic id : part3/section08/iterators_ranges_algorithms_headers
//
// Covers: headers map: iterator ranges algorithm execution

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{3, 1, 2};
    std::sort(v.begin(), v.end());
    assert(v.front() == 1);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3};
    assert(std::ranges::all_of(v, [](int x) { return x > 0; }));
}

void demo_expert() {
    std::vector<int> v{1, 2};
    auto d = std::distance(v.begin(), v.end());
    assert(d == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/iterators_ranges_algorithms_headers", run>;

}  // namespace

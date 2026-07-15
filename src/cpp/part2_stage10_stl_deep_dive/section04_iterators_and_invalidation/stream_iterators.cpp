// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : stream_iterators
// Topic id : part2/stage10/section04_iterators_and_invalidation/stream_iterators
//
// Covers: istream_iterator and ostream_iterator

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <sstream>
#include <vector>

namespace {

void demo_basics() {
    std::ostringstream oss;
    std::vector<int> v{1, 2, 3};
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(oss, " "));
    assert(oss.str() == "1 2 3 ");
}

void demo_intermediate() {
    std::istringstream iss("10 20 30");
    std::vector<int> v;
    std::copy(std::istream_iterator<int>(iss), std::istream_iterator<int>(), std::back_inserter(v));
    assert((v == std::vector<int>{10, 20, 30}));
}

void demo_expert() {
    std::istringstream iss("1 2 3 4");
    std::ostringstream oss;
    std::copy_if(std::istream_iterator<int>(iss), std::istream_iterator<int>(), std::ostream_iterator<int>(oss, ","),
                 [](int x) { return x % 2 == 0; });
    assert(oss.str() == "2,4,");
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section04_iterators_and_invalidation/stream_iterators", run>;

}  // namespace

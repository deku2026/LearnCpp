// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : iterator_concepts_cpp20
// Topic id : part2/stage10/section04_iterators_and_invalidation/iterator_concepts_cpp20
//
// Covers: C++20 iterator concepts including contiguous_iterator

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <iterator>
#include <list>
#include <vector>

namespace {

void demo_basics() {
    static_assert(std::random_access_iterator<std::vector<int>::iterator>);
    static_assert(std::bidirectional_iterator<std::list<int>::iterator>);
    static_assert(std::contiguous_iterator<std::vector<int>::iterator>);
    assert(true);
}

void demo_intermediate() {
    std::vector<int> v{10, 20, 30};
    static_assert(std::contiguous_iterator<decltype(v.begin())>);
    int* p = std::to_address(v.begin());
    assert(*p == 10);
    assert(p[2] == 30);
}

void demo_expert() {
    static_assert(std::input_iterator<std::vector<int>::iterator>);
    static_assert(std::forward_iterator<std::vector<int>::iterator>);
    static_assert(!std::contiguous_iterator<std::list<int>::iterator>);
    std::vector<int> v{1};
    auto d = std::distance(v.begin(), v.end());
    assert(d == 1);
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
    ::learn::topic<"part2/stage10/section04_iterators_and_invalidation/iterator_concepts_cpp20", run>;

}  // namespace

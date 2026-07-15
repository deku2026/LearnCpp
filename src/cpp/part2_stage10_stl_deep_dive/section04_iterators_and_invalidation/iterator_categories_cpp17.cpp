// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : iterator_categories_cpp17
// Topic id : part2/stage10/section04_iterators_and_invalidation/iterator_categories_cpp17
//
// Covers: C++17 iterator category tags and algorithm requirements

#include "learn/topic_registry.hpp"

#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

namespace {

void demo_basics() {
    using VI = std::vector<int>::iterator;
    using LI = std::list<int>::iterator;
    static_assert(std::is_same_v<std::iterator_traits<VI>::iterator_category, std::random_access_iterator_tag>);
    static_assert(std::is_same_v<std::iterator_traits<LI>::iterator_category, std::bidirectional_iterator_tag>);
    LEARN_CHECK(true);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3};
    auto it = v.begin();
    LEARN_CHECK(*(it + 2) == 3);
    LEARN_CHECK(it[1] == 2);
    std::list<int> L{1, 2, 3};
    auto lit = L.begin();
    ++lit;
    --lit;
    LEARN_CHECK(*lit == 1);
}

void demo_expert() {
    // sort requires random access — use member sort for list
    std::list<int> L{3, 1, 2};
    L.sort();
    LEARN_CHECK(L.front() == 1);
    std::vector<int> v{3, 1, 2};
    // vector iterators are random_access / contiguous-capable
    LEARN_CHECK(v.end() - v.begin() == 3);
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
    ::learn::topic<"part2/stage10/section04_iterators_and_invalidation/iterator_categories_cpp17", run>;

}  // namespace

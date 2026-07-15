// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : insert_erase_iterators
// Topic id : part2/stage10/section04_iterators_and_invalidation/insert_erase_iterators
//
// Covers: back_inserter, inserter, erase return value, erase_if

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iterator>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> src{1, 2, 3};
    std::vector<int> dst;
    std::copy(src.begin(), src.end(), std::back_inserter(dst));
    LEARN_CHECK((dst == std::vector<int>{1, 2, 3}));
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4, 5};
    for (auto it = v.begin(); it != v.end();) {
        if (*it % 2 == 0) {
            it = v.erase(it);
        } else {
            ++it;
        }
    }
    LEARN_CHECK((v == std::vector<int>{1, 3, 5}));
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::erase_if(v, [](int x) { return x % 2 == 0; });
    LEARN_CHECK((v == std::vector<int>{1, 3, 5}));
    std::vector<int> w{1, 2, 2, 3};
    std::erase(w, 2);
    LEARN_CHECK((w == std::vector<int>{1, 3}));
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
    ::learn::topic<"part2/stage10/section04_iterators_and_invalidation/insert_erase_iterators", run>;

}  // namespace

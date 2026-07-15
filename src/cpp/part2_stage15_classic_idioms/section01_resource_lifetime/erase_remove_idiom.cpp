// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : erase_remove_idiom
// Topic id : part2/stage15/section01/erase_remove_idiom
//
// Covers: erase-remove for contiguous containers

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 2, 4};
    v.erase(std::remove(v.begin(), v.end(), 2), v.end());
    LEARN_CHECK((v == std::vector<int>{1, 3, 4}));
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4, 5};
    v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }), v.end());
    LEARN_CHECK((v == std::vector<int>{1, 3, 5}));
}

void demo_expert() {
#if defined(__cpp_lib_erase_if) && __cpp_lib_erase_if >= 202002L
    std::vector<int> v{1, 1, 2, 3};
    const auto removed = std::erase(v, 1);
    LEARN_CHECK(removed == 2);
    LEARN_CHECK((v == std::vector<int>{2, 3}));
#else
    std::vector<int> v{1, 1, 2, 3};
    v.erase(std::remove(v.begin(), v.end(), 1), v.end());
    LEARN_CHECK((v == std::vector<int>{2, 3}));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/erase_remove_idiom", run>;

}  // namespace

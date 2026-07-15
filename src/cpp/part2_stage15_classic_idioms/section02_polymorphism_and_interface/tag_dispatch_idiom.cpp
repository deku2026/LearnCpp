// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : tag_dispatch_idiom
// Topic id : part2/stage15/section02/tag_dispatch_idiom
//
// Covers: tag dispatch overload selection

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iterator>
#include <type_traits>
#include <vector>

namespace {

template <class It>
int distance_impl(It first, It last, std::input_iterator_tag) {
    int n = 0;
    for (; first != last; ++first) {
        ++n;
    }
    return n;
}

template <class It>
int distance_impl(It first, It last, std::random_access_iterator_tag) {
    return static_cast<int>(last - first);
}

template <class It>
int my_distance(It first, It last) {
    using cat = typename std::iterator_traits<It>::iterator_category;
    return distance_impl(first, last, cat{});
}

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4};
    assert(my_distance(v.begin(), v.end()) == 4);
}

void demo_intermediate() {
    int a[] = {1, 2, 3};
    assert(my_distance(a, a + 3) == 3);
}

void demo_expert() {
    struct InIt {
        using iterator_category = std::input_iterator_tag;
        using value_type = int;
        using difference_type = int;
        using pointer = int*;
        using reference = int&;
        int* p;
        int& operator*() const { return *p; }
        InIt& operator++() {
            ++p;
            return *this;
        }
        bool operator!=(InIt o) const { return p != o.p; }
    };
    int a[] = {1, 2};
    assert(my_distance(InIt{a}, InIt{a + 2}) == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section02/tag_dispatch_idiom", run>;

}  // namespace

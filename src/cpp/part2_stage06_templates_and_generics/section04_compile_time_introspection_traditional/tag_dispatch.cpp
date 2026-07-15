// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : tag_dispatch
// Topic id : part2/stage06/section04/tag_dispatch
//
// Covers: tag dispatch with iterator_category / true_type false_type

#include "learn/topic_registry.hpp"

#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

namespace {

template <typename It>
int advance_impl(It, int n, std::random_access_iterator_tag) {
    return n * 100;  // O(1) path marker
}

template <typename It>
int advance_impl(It, int n, std::input_iterator_tag) {
    return n;  // O(n) path marker
}

template <typename It>
int my_advance(It it, int n) {
    using Cat = typename std::iterator_traits<It>::iterator_category;
    return advance_impl(it, n, Cat{});
}

template <typename T>
int process(T, std::true_type) {
    return 1;  // integral path
}

template <typename T>
int process(T, std::false_type) {
    return 0;
}

template <typename T>
int process(T v) {
    return process(v, std::is_integral<T>{});
}

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4};
    LEARN_CHECK(my_advance(v.begin(), 3) == 300);

    std::list<int> lst{1, 2, 3, 4};
    LEARN_CHECK(my_advance(lst.begin(), 3) == 3);
}

void demo_intermediate() {
    LEARN_CHECK(process(42) == 1);
    LEARN_CHECK(process(3.14) == 0);
    LEARN_CHECK(process(true) == 1);
}

void demo_expert() {
    static_assert(std::is_base_of_v<std::input_iterator_tag, std::random_access_iterator_tag>);
    // random_access overloads beat input via more derived tag.
    std::vector<char> c{'a', 'b'};
    LEARN_CHECK(my_advance(c.begin(), 1) == 100);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/tag_dispatch", run>;

}  // namespace

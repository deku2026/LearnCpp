// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section07_type_aliases_and_traits
// Item     : alias_template
// Topic id : part2/stage02/section07/alias_template
//
// Covers: template aliases, type trait shortcuts, partial specialization patterns

#include "learn/topic_registry.hpp"

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace {

template <class T>
using Vec = std::vector<T>;

template <class K, class V>
using Dict = std::map<K, V>;

template <class T>
using RemoveRef = std::remove_reference_t<T>;

template <class T>
using AddConst = std::add_const_t<T>;

template <class T>
using Decay = std::decay_t<T>;

template <class T, class Alloc = std::allocator<T>>
using MyVec = std::vector<T, Alloc>;

void demo_basics() {
    Vec<int> v{1, 2, 3};
    assert(v.size() == 3);
    assert(v[1] == 2);

    Dict<std::string, int> ages{{"ann", 30}, {"bob", 25}};
    assert(ages["ann"] == 30);
}

void demo_intermediate() {
    static_assert(std::is_same_v<RemoveRef<int&>, int>);
    static_assert(std::is_same_v<RemoveRef<int&&>, int>);
    static_assert(std::is_same_v<AddConst<int>, const int>);

    using CI = AddConst<int>;
    CI x = 5;
    assert(x == 5);

    Vec<std::string> names{"a", "b"};
    assert(names[0] == "a");
}

void demo_expert() {
    static_assert(std::is_same_v<Decay<const int&>, int>);
    static_assert(std::is_same_v<Decay<int[3]>, int*>);

    using Matrix = Vec<Vec<int>>;
    Matrix m{{1, 2}, {3, 4}};
    assert(m[0][1] == 2);
    assert(m[1][0] == 3);

    MyVec<int> mv{9, 8, 7};
    assert(mv.size() == 3);
    assert(mv[0] == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section07/alias_template", run>;

}  // namespace

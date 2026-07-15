// Topic    : tag dispatch —— 用空标签类型引导重载决议
// Doc      : 第2部分-阶段6 · 步骤 9.5
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : tag_dispatch
// Topic id : part2/stage06/section04/tag_dispatch
// Refs     : https://en.cppreference.com/w/cpp/types/integral_constant
//            Effective Modern C++ Item 27

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — true_type / false_type 标签分派
// ---------------------------------------------------------------------------

template <typename T>
int classify_impl(T, std::true_type) {
    return 1;  // integral
}

template <typename T>
int classify_impl(T, std::false_type) {
    return 0;  // non-integral
}

template <typename T>
int classify(T x) {
    return classify_impl(x, std::is_integral<T>{});
}

// ---------------------------------------------------------------------------
// §进阶 — 迭代器类别标签（std::distance 同款思路）
// ---------------------------------------------------------------------------

template <typename It>
auto distance_impl(It first, It last, std::random_access_iterator_tag) {
    return last - first;  // O(1)
}

template <typename It>
auto distance_impl(It first, It last, std::input_iterator_tag) {
    typename std::iterator_traits<It>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;  // O(n)
}

template <typename It>
auto my_distance(It first, It last) {
    using cat = typename std::iterator_traits<It>::iterator_category;
    return distance_impl(first, last, cat{});
}

// ---------------------------------------------------------------------------
// §专家 — 自建标签层级；与 enable_if / if constexpr 对照
// ---------------------------------------------------------------------------

struct low_priority {};
struct high_priority : low_priority {};

template <typename T>
auto dump_impl(const T& x, high_priority) -> decltype(void(x.size()), std::size_t{}) {
    return x.size();
}

template <typename T>
std::size_t dump_impl(const T&, low_priority) {
    return 0;
}

template <typename T>
std::size_t dump_size(const T& x) {
    return dump_impl(x, high_priority{});
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [tag_dispatch] 入门：integral 标签 ===\n";
    assert(classify(42) == 1);
    assert(classify(3.14) == 0);
    assert(classify('c') == 1);
    std::cout << "classify OK\n";

    std::cout << "=== 进阶：iterator_category 分派 ===\n";
    std::vector<int> v{1, 2, 3, 4, 5};
    assert(my_distance(v.begin(), v.end()) == 5);
    std::list<int> lst{1, 2, 3};
    assert(my_distance(lst.begin(), lst.end()) == 3);
    std::cout << "my_distance vector/list OK\n";

    std::cout << "=== 专家：优先级标签 + 与现代手段对照 ===\n";
    assert(dump_size(v) == 5);
    assert(dump_size(42) == 0);
    // tag dispatch 优点：实现分离清晰、无 SFINAE 噪音
    // if constexpr 更适合「同一函数内分支」
    // Concepts 更适合「约束重载集」
    // 标准库历史代码大量使用 iterator tag dispatch
    std::cout << "=== tag_dispatch: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/tag_dispatch", run>;

}  // namespace

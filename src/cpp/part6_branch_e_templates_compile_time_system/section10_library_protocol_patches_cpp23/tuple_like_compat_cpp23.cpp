// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E10 C++23 protocol)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : tuple_like_compat_cpp23
// Topic id : part6/e/section10/tuple_like_compat_cpp23
//
// 要点: C++23 扩展 tuple-like 协议互通 —— pair/array/tuple/子范围等
//       在 apply / 结构化绑定 / 构造中更一致。
// 参考: P2165R4; cppreference tuple-like

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace {

// 用户类型 opt-in tuple 协议 (C++11 起可用, C++23 与库算法更互通)
struct Point {
    int x;
    int y;
};

}  // namespace

// 必须在全局 / 与类型关联的命名空间特化
template <>
struct std::tuple_size<Point> : std::integral_constant<std::size_t, 2> {};

template <std::size_t I>
struct std::tuple_element<I, Point> {
    using type = int;
};

namespace {

template <std::size_t I>
constexpr int get(const Point& p) {
    if constexpr (I == 0) {
        return p.x;
    } else {
        return p.y;
    }
}

template <std::size_t I>
constexpr int& get(Point& p) {
    if constexpr (I == 0) {
        return p.x;
    } else {
        return p.y;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E10 tuple-like compatibility (C++23) ===\n";

    // pair / tuple / array 都是 tuple-like
    std::pair<int, std::string> pr{1, "a"};
    std::tuple<int, std::string> tp = pr;  // C++23 更宽松的 tuple-like 构造
    assert(std::get<0>(tp) == 1);
    assert(std::get<1>(tp) == "a");

    std::array<int, 2> ar{10, 20};
    auto [a0, a1] = ar;
    assert(a0 == 10 && a1 == 20);

    // apply 对 tuple-like
    auto sum = std::apply([](int x, int y) { return x + y; }, ar);
    assert(sum == 30);

    // 用户 Point
    Point p{3, 4};
    auto [x, y] = p;
    assert(x == 3 && y == 4);
    assert(get<0>(p) == 3);

    // pair ← array (元素兼容时)
    std::pair<int, int> pr2 = std::pair{ar[0], ar[1]};
    assert(pr2.first + pr2.second == 30);

    // tuple_cat 经典
    auto cat = std::tuple_cat(std::make_tuple(1), std::make_tuple(2, 3));
    assert(std::get<2>(cat) == 3);

    std::cout << "  protocol: tuple_size / tuple_element / get<I>\n";
    std::cout << "  C++23: library algorithms treat more types as tuple-like\n";
    std::cout << "tuple_like_compat_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section10/tuple_like_compat_cpp23", run>;

}  // namespace

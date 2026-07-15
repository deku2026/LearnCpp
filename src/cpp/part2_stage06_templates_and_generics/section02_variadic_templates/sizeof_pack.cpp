// Topic    : sizeof...(pack) —— 编译期包长度
// Doc      : 第2部分-阶段6 · 步骤 7.2
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : sizeof_pack
// Topic id : part2/stage06/section02/sizeof_pack
// Refs     : https://en.cppreference.com/w/cpp/language/sizeof...
//            ISO [expr.sizeof]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <initializer_list>
#include <iostream>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — sizeof...(Args) 与 sizeof...(args)
// ---------------------------------------------------------------------------

template <typename... Args>
constexpr std::size_t arity() {
    return sizeof...(Args);
}

template <typename... Args>
constexpr std::size_t arity_of_values(Args... args) {
    ((void)args, ...);  // 使用包，避免未使用警告
    return sizeof...(Args);
}

// ---------------------------------------------------------------------------
// §进阶 — 用包长度驱动数组 / 静态断言
// ---------------------------------------------------------------------------

template <typename... Ts>
constexpr auto make_type_sizes() {
    return std::array<std::size_t, sizeof...(Ts)>{sizeof(Ts)...};
}

template <typename T, typename... Rest>
constexpr bool first_is_largest() {
    constexpr std::size_t head = sizeof(T);
    constexpr std::size_t rest_max = sizeof...(Rest) == 0 ? 0 : (std::max)({sizeof(Rest)...});  // initializer_list max
    return head >= rest_max;
}

// ---------------------------------------------------------------------------
// §专家 — sizeof... 是常量表达式；与 sizeof(expr) 不同
// ---------------------------------------------------------------------------

template <typename... Args>
void require_non_empty(Args... args) {
    static_assert(sizeof...(Args) > 0, "pack must not be empty");
    // 使用 args 避免 unused（空包走不到这里）
    ((void)args, ...);
}

template <std::size_t N>
struct Dim {
    static constexpr std::size_t value = N;
};

template <typename... Ts>
using dim_of = Dim<sizeof...(Ts)>;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [sizeof_pack] 入门 ===\n";
    static_assert(arity<>() == 0);
    static_assert(arity<int, double>() == 2);
    assert(arity_of_values() == 0);
    assert(arity_of_values(1, 2, 3, 4) == 4);
    std::cout << "arity 4=" << arity_of_values(1, 2, 3, 4) << '\n';

    std::cout << "=== 进阶：包长度作数组界 ===\n";
    constexpr auto sizes = make_type_sizes<char, int, double>();
    static_assert(sizes.size() == 3);
    assert(sizes[0] == 1);
    assert(sizes[1] == sizeof(int));
    assert(sizes[2] == sizeof(double));
    static_assert(first_is_largest<double, char, int>());
    static_assert(!first_is_largest<char, int, double>());
    std::cout << "type sizes array OK\n";

    std::cout << "=== 专家：与 sizeof 区分；static_assert 护栏 ===\n";
    require_non_empty(1, 2);
    static_assert(dim_of<int, int, int>::value == 3);
    // sizeof...(pack) 不求值包元素；sizeof(expr) 求类型大小
    [[maybe_unused]] int x = 0;
    assert(sizeof(x) == sizeof(int));
    assert(arity_of_values(x, x) == 2);
    std::cout << "=== sizeof_pack: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/sizeof_pack", run>;

}  // namespace

// Topic    : 标准 concepts 库 `<concepts>`
// Doc      : 第2部分-阶段6 · 步骤 11.4
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : standard_concepts_library
// Topic id : part2/stage06/section05/standard_concepts_library
// Refs     : https://en.cppreference.com/w/cpp/concepts
//            ISO [concepts]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 核心语言概念
// ---------------------------------------------------------------------------

template <std::integral T>
T id_i(T x) {
    return x;
}

template <std::floating_point T>
T id_f(T x) {
    return x;
}

// ---------------------------------------------------------------------------
// §进阶 — same_as / convertible_to / derived_from / invocable
// ---------------------------------------------------------------------------

struct Base {
    virtual ~Base() = default;
};
struct Der : Base {};

template <typename F, typename... Args>
    requires std::invocable<F, Args...>
auto call(F&& f, Args&&... args) {
    return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
}

template <std::convertible_to<std::string> T>
std::string as_string(T&& x) {
    return std::string(std::forward<T>(x));
}

// ---------------------------------------------------------------------------
// §专家 — 对象语义概念；totally_ordered；与 ranges 衔接
// ---------------------------------------------------------------------------

template <std::copyable T>
T clone(const T& x) {
    return x;
}

template <std::totally_ordered T>
const T& max2(const T& a, const T& b) {
    return (b < a) ? a : b;
}

template <typename T>
void show_predicates() {
    std::cout << "  same_as<T,int>=" << std::same_as<T, int> << " movable="
              << std::movable<T> << " equality_comparable=" << std::equality_comparable<T> << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [standard_concepts_library] 入门 ===\n";
    assert(id_i(7) == 7);
    assert(id_f(1.5) == 1.5);
    static_assert(std::integral<int>);
    static_assert(std::signed_integral<int>);
    static_assert(std::unsigned_integral<unsigned>);
    static_assert(std::floating_point<double>);
    std::cout << "integral/floating_point OK\n";

    std::cout << "=== 进阶：关系与可调用 ===\n";
    static_assert(std::same_as<int, int>);
    static_assert(std::convertible_to<int, double>);
    static_assert(std::derived_from<Der, Base>);
    static_assert(!std::derived_from<Base, Der>);
    assert(call([](int a, int b) { return a * b; }, 6, 7) == 42);
    assert(as_string("hi") == "hi");
    assert(as_string(std::string{"z"}) == "z");
    std::cout << "same_as/convertible/derived/invocable OK\n";

    std::cout << "=== 专家：copyable / totally_ordered ===\n";
    assert(clone(std::string{"x"}) == "x");
    assert(max2(3, 5) == 5);
    assert(max2(std::string{"a"}, std::string{"b"}) == "b");
    static_assert(std::movable<std::string>);
    static_assert(std::copyable<int>);
    static_assert(std::equality_comparable<int>);
    static_assert(std::totally_ordered<int>);
    show_predicates<int>();
    show_predicates<std::string>();
    // <iterator> / <ranges> 另有 input_iterator、range 等大量概念
    std::cout << "=== standard_concepts_library: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/standard_concepts_library", run>;

}  // namespace

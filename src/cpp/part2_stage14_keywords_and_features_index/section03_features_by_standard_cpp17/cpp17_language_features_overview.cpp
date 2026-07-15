// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section03_features_by_standard_cpp17
// Item     : cpp17_language_features_overview
// Topic id : part2/stage14/section03/cpp17_language_features_overview
//
// C++17 language micro-matrix. Anchors: __cpp_structured_bindings, __cpp_if_constexpr,
// __cpp_fold_expressions, __cpp_inline_variables, __cpp_nontype_template_parameter_auto,
// __cpp_capture_star_this, __cpp_guaranteed_copy_elision, __has_include, ...

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// fold expressions
template <class... Ts>
auto fold_sum(Ts... xs) {
    return (xs + ...);
}
template <class... Ts>
void fold_print(const Ts&... xs) {
    ((std::cout << xs << ' '), ...);
    std::cout << '\n';
}

// if constexpr + auto NTTP
template <auto N>
constexpr auto nttp_value() {
    if constexpr (std::is_integral_v<decltype(N)>) {
        return N + N;
    } else {
        return N;
    }
}

// CTAD + deduction guide (custom)
template <class T>
struct Wrap {
    T value;
    Wrap(T v) : value(std::move(v)) {}
};
// CTAD works for Wrap{42}

// inline variables
struct Config {
    static inline int counter = 0;
};

// *this by value capture
struct Counter {
    int n = 0;
    auto make_snapshot() const {
        return [*this] { return n; };
    }
};

// [[fallthrough]] / [[nodiscard]] / [[maybe_unused]]
[[nodiscard]] int must_use() {
    return 7;
}

int classify(int x) {
    switch (x) {
        case 1:
            [[fallthrough]];
        case 2:
            return 12;
        default:
            return 0;
    }
}

// guaranteed copy elision: prvalue construction
struct NoCopyMove {
    int v;
    explicit NoCopyMove(int x) : v(x) {}
    NoCopyMove(const NoCopyMove&) = delete;
    NoCopyMove(NoCopyMove&&) = delete;
};
NoCopyMove make_ncm() {
    return NoCopyMove{99};
}

// nested namespace definition
namespace a::b::c {
inline int depth() {
    return 3;
}
}  // namespace a::b::c

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++17 language features micro-matrix ===\n";

    // 1) structured bindings + if/switch with initializer
    std::map<std::string, int> m{{"x", 1}, {"y", 2}};
    for (const auto& [k, v] : m) {
        std::cout << "  map " << k << "->" << v << '\n';
    }
    if (auto it = m.find("x"); it != m.end()) {
        std::cout << "[structured bindings / if-init] x=" << it->second << '\n';
        assert(it->second == 1);
    }
    switch (int code = classify(1); code) {
        case 12:
            std::cout << "[switch-init/fallthrough] code=" << code << '\n';
            break;
        default:
            assert(false);
    }

    // 2) if constexpr
    auto i2 = nttp_value<21>();
    auto d2 = nttp_value<3.0>();
    std::cout << "[if constexpr / auto NTTP] i2=" << i2 << " d2=" << d2 << '\n';
    assert(i2 == 42);

    // 3) fold expressions
    const auto s = fold_sum(1, 2, 3, 4, 5);
    std::cout << "[fold] sum=" << s << " print: ";
    fold_print("a", 1, 2.5);
    assert(s == 15);

    // 4) inline variables / CTAD
    ++Config::counter;
    ++Config::counter;
    Wrap w{std::string{"ctad"}};
    std::cout << "[inline var / CTAD] counter=" << Config::counter << " wrap=" << w.value << '\n';
    assert(Config::counter == 2);

    // 5) constexpr lambda + *this capture
    constexpr auto cl = [](int x) constexpr { return x * x; };
    static_assert(cl(4) == 16);
    Counter ctr{5};
    auto snap = ctr.make_snapshot();
    ctr.n = 9;
    std::cout << "[constexpr lambda / *this capture] cl(4)=" << cl(4) << " snapshot=" << snap() << " now=" << ctr.n
              << '\n';
    assert(snap() == 5);

    // 6) attributes
    [[maybe_unused]] int unused_demo = 0;
    int used = must_use();
    std::cout << "[attributes] nodiscard value=" << used << '\n';
    assert(used == 7);

    // 7) guaranteed copy elision
    NoCopyMove ncm = make_ncm();
    std::cout << "[guaranteed elision] ncm.v=" << ncm.v << '\n';
    assert(ncm.v == 99);

    // 8) nested namespace + __has_include
    std::cout << "[nested ns] depth=" << a::b::c::depth() << '\n';
#if __has_include(<optional>)
    std::cout << "[__has_include] <optional> present\n";
#else
    std::cout << "[__has_include] <optional> missing\n";
#endif

    // 9) enum direct-list-init (C++17)
    enum class E : int {};
    E e{5};
    std::cout << "[enum list-init] static_cast<int>(e)=" << static_cast<int>(e) << '\n';
    assert(static_cast<int>(e) == 5);

    // 10) selection with structured binding in init (C++20 extends more; C++17 tuple bind)
    auto tup = std::make_tuple(3, 4);
    auto [tx, ty] = tup;
    std::cout << "[tuple binding] (" << tx << ',' << ty << ")\n";
    assert(tx == 3 && ty == 4);

    std::cout << "C++17 language matrix OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section03/cpp17_language_features_overview", run>;

}  // namespace

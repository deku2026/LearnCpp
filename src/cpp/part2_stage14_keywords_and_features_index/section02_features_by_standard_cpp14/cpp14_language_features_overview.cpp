// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section02_features_by_standard_cpp14
// Item     : cpp14_language_features_overview
// Topic id : part2/stage14/section02/cpp14_language_features_overview
//
// C++14 language micro-matrix. Feature-test anchors: __cpp_generic_lambdas,
// __cpp_init_captures, __cpp_return_type_deduction, __cpp_decltype_auto,
// __cpp_variable_templates, __cpp_binary_literals, __cpp_digit_separators, ...

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// auto return type deduction (C++14)
auto mul(int a, int b) {
    return a * b;
}

// decltype(auto) preserves reference-ness
int g_val = 100;
decltype(auto) get_g_ref() {
    return (g_val);
}  // lvalue ref to g_val

// relaxed constexpr (C++14): local vars, loops
constexpr int factorial(int n) {
    int r = 1;
    for (int i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}
static_assert(factorial(5) == 120, "C++14 relaxed constexpr");

// variable templates
template <class T>
constexpr T pi_v = T(3.1415926535897932385L);

// [[deprecated]]
[[deprecated("use new_api")]] int old_api() {
    return -1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++14 language features micro-matrix ===\n";

    // 1) generic lambda
    auto generic_add = [](auto x, auto y) { return x + y; };
    std::cout << "[generic lambda] 2+3=" << generic_add(2, 3) << " 1.5+2.5=" << generic_add(1.5, 2.5) << '\n';
    assert(generic_add(2, 3) == 5);

    // 2) init-capture (move capture)
    std::vector<int> payload{1, 2, 3};
    auto consumer = [v = std::move(payload)]() { return static_cast<int>(v.size()); };
    assert(payload.empty());
    std::cout << "[init-capture] moved size=" << consumer() << " leftover=" << payload.size() << '\n';

    // 3) auto return / decltype(auto)
    assert(mul(6, 7) == 42);
    get_g_ref() = 200;
    std::cout << "[auto return / decltype(auto)] mul=" << mul(6, 7) << " g_val=" << g_val << '\n';
    assert(g_val == 200);

    // 4) relaxed constexpr
    constexpr int f6 = factorial(6);
    std::cout << "[relaxed constexpr] 6!=" << f6 << '\n';
    assert(f6 == 720);

    // 5) variable templates
    std::cout << "[variable templates] pi_v<float>≈" << pi_v<float> << " pi_v<int>=" << pi_v<int> << '\n';
    assert(pi_v<int> == 3);

    // 6) binary literals + digit separators
    const int flags = 0b1010'1100;
    const long big = 1'000'000;
    std::cout << "[0b / digit sep] flags=" << flags << " big=" << big << '\n';
    assert(flags == 172 && big == 1000000);

    // 7) [[deprecated]] still callable (may warn)
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    const int deprecated_result = old_api();
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif
    std::cout << "[deprecated attr] old_api()=" << deprecated_result << " (discouraged)\n";

    // 8) aggregate NSDMI (C++14 allows aggregates with default member initializers)
    struct Agg {
        int x = 1;
        int y = 2;
    };
    Agg a{};
    std::cout << "[aggregate NSDMI] a=(" << a.x << ',' << a.y << ")\n";
    assert(a.x == 1 && a.y == 2);

    std::cout << "C++14 language matrix OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section02/cpp14_language_features_overview", run>;

}  // namespace

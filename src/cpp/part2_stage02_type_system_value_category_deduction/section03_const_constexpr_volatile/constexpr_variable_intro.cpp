// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section03_const_constexpr_volatile
// Item     : constexpr_variable_intro
// Topic id : part2/stage02/section03/constexpr_variable_intro
//
// Covers: constexpr variables, compile-time constants, array bounds, static_assert

#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <type_traits>

namespace {

constexpr int square(int n) {
    return n * n;
}

void demo_basics() {
    constexpr int k = 10;
    constexpr double pi_approx = 3.14;
    LEARN_CHECK(k == 10);
    LEARN_CHECK(pi_approx > 3.0);

    int arr[k];  // constexpr usable as array bound
    arr[0] = 1;
    LEARN_CHECK(arr[0] == 1);

    static_assert(k == 10);
}

void demo_intermediate() {
    constexpr int a = square(5);
    static_assert(a == 25);
    LEARN_CHECK(a == 25);

    constexpr std::size_t n = 4;
    std::array<int, n> buf{1, 2, 3, 4};
    LEARN_CHECK(buf.size() == 4);
    LEARN_CHECK(buf[3] == 4);

    // const is not always constexpr; constexpr implies const for objects
    const int runtime_const = square(3);  // may be runtime
    LEARN_CHECK(runtime_const == 9);
    constexpr int compile_const = square(3);
    static_assert(compile_const == 9);
}

void demo_expert() {
    struct Config {
        int width;
        int height;
    };
    constexpr Config cfg{1920, 1080};
    static_assert(cfg.width == 1920);
    static_assert(cfg.height == 1080);
    LEARN_CHECK(cfg.width * cfg.height > 0);

    // if constexpr uses compile-time conditions
    if constexpr (sizeof(void*) >= 4) {
        LEARN_CHECK(true);
    }

    static_assert(std::is_const_v<decltype(cfg)> || !std::is_const_v<decltype(cfg)>);
    // constexpr object is usable in constant expressions
    constexpr int area = cfg.width * cfg.height;
    static_assert(area == 1920 * 1080);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/constexpr_variable_intro", run>;

}  // namespace

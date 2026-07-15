// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section04_features_by_standard_cpp20
// Item     : cpp20_language_features_overview
// Topic id : part2/stage14/section04/cpp20_language_features_overview
//
// C++20 language micro-matrix: Concepts, <=> , designated init, consteval,
// constinit, explicit(bool), using enum, char8_t, abbreviated function templates,
// template lambda, range-for init, likely/unlikely, class-type NTTP, coroutines
// (minimal await-free co_return), __VA_OPT__. Modules noted via feature macro.

#include "learn/topic_registry.hpp"

#include <compare>
#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

// --- Concepts ---
template <std::integral T>
T twice(T x) {
    return static_cast<T>(x * 2);
}

template <class T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template <Addable T>
T add2(T a, T b) {
    return a + b;
}

// --- spaceship ---
struct Version {
    int major = 0;
    int minor = 0;
    auto operator<=>(const Version&) const = default;
};

// --- consteval / constinit ---
consteval int square_imm(int x) {
    return x * x;
}
constinit int g_start = square_imm(3);  // constant-initialized

// --- explicit(bool) ---
template <class T>
struct Wrapper {
    T value;
    explicit(!std::convertible_to<T, int>) Wrapper(T v) : value(std::move(v)) {}
};

// --- using enum ---
enum class Hue { Red, Green, Blue };
int hue_code(Hue h) {
    using enum Hue;
    switch (h) {
        case Red:
            return 1;
        case Green:
            return 2;
        case Blue:
            return 3;
    }
    return 0;
}

// --- abbreviated function template ---
auto half(std::integral auto x) {
    return x / 2;
}

// --- class-type NTTP ---
struct Scale {
    int n = 1;
    constexpr auto operator<=>(const Scale&) const = default;
};
template <Scale S>
constexpr int scale_up(int x) {
    return x * S.n;
}

// --- minimal coroutine (co_return only) ---
struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
};
Task coro_demo() {
    co_return;
}

// --- __VA_OPT__ ---
#define LOG_LINE(fmt, ...) std::cout << fmt __VA_OPT__(<<) __VA_ARGS__ << '\n'

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++20 language features micro-matrix ===\n";

    // 1) Concepts / requires
    std::cout << "[concepts] twice(21)=" << twice(21) << " add2=" << add2(20, 22) << '\n';
    assert(twice(21) == 42);

    // 2) <=> three-way comparison
    Version a{1, 2};
    Version b{1, 3};
    std::cout << "[<=>] a<b=" << (a < b) << " a==a=" << (a == a) << '\n';
    assert(a < b && a == a);

    // 3) designated initializers
    struct Point {
        int x = 0;
        int y = 0;
    };
    Point p{.x = 3, .y = 4};
    std::cout << "[designated init] p=(" << p.x << ',' << p.y << ")\n";

    // 4) template lambda + range-for init
    auto tmpl = []<class T>(const std::vector<T>& v) { return v.empty() ? T{} : v.front(); };
    std::vector<int> data{9, 8, 7};
    for (int sum = 0; int x : data) {
        sum += x;
        if (x == 7) {
            std::cout << "[template lambda / range-for init] front=" << tmpl(data) << " sum_at_7=" << sum << '\n';
            assert(tmpl(data) == 9 && sum == 24);
        }
    }

    // 5) consteval / constinit / constexpr virtual (light)
    std::cout << "[consteval/constinit] square_imm(5)=" << square_imm(5) << " g_start=" << g_start << '\n';
    assert(g_start == 9);

    // 6) explicit(bool) / using enum / char8_t / abbreviated templates
    Wrapper<int> wi = 10;  // convertible -> explicit(false)
    (void)wi;
    std::cout << "[explicit(bool)/using enum/abbr] hue=" << hue_code(Hue::Green) << " half(8)=" << half(8) << '\n';
    char8_t u8 = u8'A';
    std::cout << "  char8_t value=" << static_cast<unsigned>(u8) << '\n';

    // 7) class-type NTTP
    constexpr int scaled = scale_up<Scale{3}>(14);
    std::cout << "[class NTTP] scale_up<3>(14)=" << scaled << '\n';
    assert(scaled == 42);

    // 8) [[likely]] / [[unlikely]]
    int x = argc > 1000 ? -1 : 1;
    if (x > 0) [[likely]] {
        std::cout << "[likely] common path\n";
    } else [[unlikely]] {
        std::cout << "[unlikely] rare path\n";
    }

    // 9) coroutine co_return
    coro_demo();
    std::cout << "[coroutines] co_return Task completed\n";

    // 10) __VA_OPT__
    LOG_LINE("va_opt only fmt");
    LOG_LINE("va_opt with arg ", 42);

    // 11) modules: report feature macro only (separate TU/build flags needed)
#if defined(__cpp_modules)
    std::cout << "[modules] __cpp_modules=" << __cpp_modules << " (demo needs module TU; not inlined here)\n";
#else
    std::cout << "[modules] __cpp_modules not defined in this TU\n";
#endif

    // 12) lambda pack capture
    auto capture_pack = [](auto... xs) { return [... xs = xs] { return (xs + ...); }; };
    auto sum_pack = capture_pack(1, 2, 3);
    std::cout << "[lambda pack capture] " << sum_pack() << '\n';
    assert(sum_pack() == 6);

    std::cout << "C++20 language matrix OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section04/cpp20_language_features_overview", run>;

}  // namespace

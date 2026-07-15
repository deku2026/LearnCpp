// Topic    : headers map · 工具 utility
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : utility_headers
// Topic id : part3/section08/utility_headers
// Headers  : <utility> <tuple> <optional> <expected> <variant> <any>
//            <functional> <bit> <version>

#include "learn/topic_registry.hpp"

#include <any>
#include <bit>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <version>

#if defined(__has_include)
#if __has_include(<expected>)
#include <expected>
#define LEARN_HAS_EXPECTED 1
#endif
#endif
#ifndef LEARN_HAS_EXPECTED
#define LEARN_HAS_EXPECTED 0
#endif

namespace {

int add(int a, int b) {
    return a + b;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [utility_headers] map probe ===\n";
    std::cout << "<utility>    move/forward/pair/exchange/to_underlying/unreachable\n";
    std::cout << "<tuple>      tuple/apply/get\n";
    std::cout << "<optional>   optional monadic API\n";
    std::cout << "<expected>   C++23 success|error\n";
    std::cout << "<variant>    type-safe union\n";
    std::cout << "<any>        type-erased value\n";
    std::cout << "<functional> function/bind_front/invoke\n";
    std::cout << "<bit>        popcount/byteswap (C++20/23)\n";
    std::cout << "<version>    feature-test macros\n";

    [[maybe_unused]] auto p = std::pair{1, 2};
    assert(std::exchange(p.first, 9) == 1 && p.first == 9);

    [[maybe_unused]] auto t = std::tuple{1, 2, 3};
    assert(std::apply([](int a, int b, int c) { return a + b + c; }, t) == 6);

    [[maybe_unused]] std::optional<int> o = 42;
    assert(o.value_or(0) == 42);

#if LEARN_HAS_EXPECTED
    std::expected<int, std::string> e = 7;
    assert(e.has_value() && *e == 7);
    std::cout << "expected value=" << *e << '\n';
#else
    std::cout << "<expected> not available\n";
#endif

    std::variant<int, std::string> v = std::string{"hi"};
    assert(std::holds_alternative<std::string>(v));

    std::any a = 3.14;
    assert(std::any_cast<double>(a) == 3.14);

    std::function<int(int, int)> f = add;
    assert(f(2, 3) == 5);
    [[maybe_unused]] auto front = std::bind_front(add, 10);
    assert(front(5) == 15);

    assert(std::popcount(0b1011u) == 3);
    std::cout << "popcount(0b1011)=" << std::popcount(0b1011u) << '\n';

#ifdef __cplusplus
    std::cout << "__cplusplus=" << __cplusplus << '\n';
#endif
#if defined(__cpp_lib_expected)
    std::cout << "__cpp_lib_expected=" << __cpp_lib_expected << '\n';
#endif

    std::cout << "[utility_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/utility_headers", run>;

}  // namespace

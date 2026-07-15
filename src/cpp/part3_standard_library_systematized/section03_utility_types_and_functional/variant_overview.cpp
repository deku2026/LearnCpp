// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.1 variant
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : variant_overview
// Topic id : part3/section03/variant_overview
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/variant
//
// 要点: 类型安全联合(和类型); index/holds_alternative/get; visit 是正规访问方式。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <variant>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/variant_overview ===\n";

    std::variant<int, std::string> v = 42;
    assert(std::holds_alternative<int>(v));
    assert(v.index() == 0);
    assert(std::get<int>(v) == 42);
    assert(std::get<0>(v) == 42);

    v = std::string{"hello"};
    assert(std::holds_alternative<std::string>(v));
    assert(std::get<std::string>(v) == "hello");

    // get_if: 不抛
    if (auto* p = std::get_if<std::string>(&v)) {
        (void)p;
        assert(*p == "hello");
    }

    // 错误类型 get 抛 bad_variant_access
    [[maybe_unused]] bool threw = false;
    try {
        (void)std::get<int>(v);
    } catch (const std::bad_variant_access&) {
        threw = true;
    }
    assert(threw);

    std::cout << "[intro] holds_alternative / get / get_if ok\n";

    // valueless_by_exception 罕见路径: 赋值中抛可能导致
    // 日常代码用 emplace 更安全
    v.emplace<int>(7);
    assert(std::get<int>(v) == 7);

    std::cout << "[advanced] emplace avoids temporary issues\n";
    std::cout << "[expert] prefer visit over manual index switches\n";
    std::cout << "variant_overview: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/variant_overview", run>;

}  // namespace

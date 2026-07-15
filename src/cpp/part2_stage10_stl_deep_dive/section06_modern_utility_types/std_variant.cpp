// Topic     : std::variant —— 类型安全联合体
// Doc       : 第2部分-阶段10 · 步骤 9.2
// cppreference: https://en.cppreference.com/cpp/utility/variant
//
// 要点: get / get_if / holds_alternative / index; 错类型抛 bad_variant_access。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_variant] ===\n";

    std::variant<int, std::string, double> v{42};
    assert(std::holds_alternative<int>(v));
    assert(v.index() == 0);
    assert(std::get<int>(v) == 42);
    assert(std::get<0>(v) == 42);

    v = std::string{"hello"};
    assert(std::holds_alternative<std::string>(v));
    assert(std::get<std::string>(v) == "hello");

    if (auto* p = std::get_if<std::string>(&v)) {
        (void)p;
        assert(*p == "hello");
    } else {
        assert(false);
    }
    assert(std::get_if<int>(&v) == nullptr);

    v = 3.14;
    assert(std::get<double>(v) == 3.14);

    // 错类型 → 异常
    [[maybe_unused]] bool threw = false;
    try {
        (void)std::get<int>(v);
    } catch (const std::bad_variant_access&) {
        threw = true;
    }
    assert(threw);

    // emplace
    v.emplace<std::string>(3, 'z');
    assert(std::get<std::string>(v) == "zzz");

    // valueless_by_exception 罕见, 正常路径为 false
    assert(!v.valueless_by_exception());

    std::cout << "[variant] get/get_if/holds/index/bad_access OK\n";
    std::cout << "std_variant: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_variant", run>;

}  // namespace

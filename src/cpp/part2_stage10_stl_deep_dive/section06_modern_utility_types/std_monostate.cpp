// Topic     : std::monostate —— variant 的空占位类型
// Doc       : 第2部分-阶段10 · 步骤 9.2
// cppreference: https://en.cppreference.com/cpp/utility/variant/monostate
//
// 要点: 让"首类型不可默认构造"的 variant 可默认构造; 表示空状态。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <variant>

namespace {

struct NoDefault {
    int x;
    explicit NoDefault(int v) : x(v) {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_monostate] ===\n";

    // 若首类型不可默认构造, variant 也不能默认构造:
    // std::variant<NoDefault, std::string> bad; // ill-formed

    // monostate 作首类型 → 默认可构造, 表示"空"
    std::variant<std::monostate, NoDefault, std::string> v;
    assert(std::holds_alternative<std::monostate>(v));
    assert(v.index() == 0);

    v = NoDefault{7};
    assert(std::get<NoDefault>(v).x == 7);

    v = std::string{"hi"};
    assert(std::get<std::string>(v) == "hi");

    v = std::monostate{};
    assert(std::holds_alternative<std::monostate>(v));

    // monostate 可比较, 全相等
    assert(std::monostate{} == std::monostate{});

    std::cout << "[monostate] empty state + non-default-constructible alt OK\n";
    std::cout << "std_monostate: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_monostate", run>;

}  // namespace

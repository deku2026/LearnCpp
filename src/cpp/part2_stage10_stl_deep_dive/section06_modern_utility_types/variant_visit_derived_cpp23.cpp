// Topic     : visit 支持派生自 variant 的类 (C++23, P2162)
// Doc       : 第2部分-阶段10 · 步骤 9.2
// cppreference: https://en.cppreference.com/cpp/utility/variant/visit
//
// 要点: 继承 variant 的强类型包装可直接 std::visit。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <variant>

namespace {

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

// 强类型: 派生自 variant
class JsonValue : public std::variant<std::nullptr_t, bool, double, std::string> {
public:
    using variant::variant;
};

[[maybe_unused]] std::string stringify(const JsonValue& j) {
    return std::visit(overloaded{
                          [](std::nullptr_t) { return std::string{"null"}; },
                          [](bool b) { return b ? std::string{"true"} : std::string{"false"}; },
                          [](double d) { return std::to_string(d); },
                          [](const std::string& s) { return '"' + s + '"'; },
                      },
                      j);  // C++23: 可直接 visit 派生类
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [variant_visit_derived_cpp23] ===\n";

    JsonValue n{nullptr};
    JsonValue b{true};
    JsonValue num{3.0};
    JsonValue str{std::string{"hi"}};

    assert(stringify(n) == "null");
    assert(stringify(b) == "true");
    assert(stringify(str) == "\"hi\"");
    assert(stringify(num).starts_with("3"));

    // 赋值与 index
    num = std::string{"x"};
    assert(num.index() == 3);
    assert(stringify(num) == "\"x\"");

    std::cout << "[P2162] visit(derived-from-variant) OK\n";
    std::cout << "variant_visit_derived_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/variant_visit_derived_cpp23", run>;

}  // namespace

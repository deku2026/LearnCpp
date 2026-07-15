// Topic    : std::variant 承载多种结果/错误 + visit 模式分发
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 6.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_variant_as_error
// Topic id : part2/stage09/section05/std_variant_as_error
// Refs     : https://en.cppreference.com/w/cpp/utility/variant
//            https://en.cppreference.com/w/cpp/utility/variant/visit

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

namespace {

struct Success {
    int value;
};
struct NotFound {};
struct PermissionDenied {
    std::string user;
};

using FetchResult = std::variant<Success, NotFound, PermissionDenied>;

FetchResult fetch(int id) {
    if (id == 1) {
        return Success{100};
    }
    if (id == 2) {
        return PermissionDenied{"bob"};
    }
    return NotFound{};
}

// overloaded 惯用法（阶段 8）
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_variant_as_error] 主干：visit 穷尽分派 ===\n";
    {
        std::string out;
        for (int id : {1, 2, 3}) {
            std::visit(overloaded{
                           [&](Success s) { out += "ok:" + std::to_string(s.value) + ";"; },
                           [&](NotFound) { out += "missing;"; },
                           [&](PermissionDenied p) { out += "deny:" + p.user + ";"; },
                       },
                       fetch(id));
        }
        assert(out == "ok:100;deny:bob;missing;");
        std::cout << out << '\n';
    }

    std::cout << "=== 对抗：holds_alternative / get_if ===\n";
    {
        auto r = fetch(1);
        assert(std::holds_alternative<Success>(r));
        if (auto* s = std::get_if<Success>(&r)) {
            assert(s->value == 100);
        }
        std::cout << "get_if Success value=100\n";
    }

    std::cout << "=== 专节：variant vs expected ===\n";
    // expected：一个值 or 一个错误类型
    // variant：多个可能类型（多种成功/多种错误），需 visit 穷尽
    // 错误种类多且要分别处理 → variant；单一业务错误通道 → expected
    std::cout << "variant = closed set of outcomes; expected = T|E\n";

    std::cout << "[std_variant_as_error] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section05/std_variant_as_error", run>;

}  // namespace

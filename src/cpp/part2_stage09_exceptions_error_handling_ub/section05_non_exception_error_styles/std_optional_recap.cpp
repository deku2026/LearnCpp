// Topic    : std::optional 复盘：有/无语义
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 5.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_optional_recap
// Topic id : part2/stage09/section05/std_optional_recap
// Refs     : https://en.cppreference.com/w/cpp/utility/optional
//            ISO [optional]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <string>

namespace {

std::optional<int> find_user(const std::string& name) {
    if (name == "Alice") {
        return 42;
    }
    return std::nullopt;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_optional_recap] 主干：有值 / 空 ===\n";
    {
        auto u = find_user("Alice");
        assert(u.has_value());
        assert(static_cast<bool>(u));
        assert(*u == 42);
        assert(u.value() == 42);
        std::cout << "Alice id=" << *u << '\n';

        auto missing = find_user("Bob");
        assert(!missing);
        assert(missing.value_or(-1) == -1);
        std::cout << "Bob -> " << missing.value_or(-1) << '\n';
    }

    std::cout << "=== 对抗：value() 空时抛 bad_optional_access ===\n";
    {
        std::optional<int> empty;
        [[maybe_unused]] bool threw = false;
        try {
            (void)empty.value();
        } catch (const std::bad_optional_access&) {
            threw = true;
        }
        assert(threw);
        std::cout << "value() on nullopt throws bad_optional_access\n";
    }

    std::cout << "=== 对抗：emplace / reset ===\n";
    {
        std::optional<std::string> o;
        o.emplace(3, 'x');  // "xxx"
        assert(o && *o == "xxx");
        o.reset();
        assert(!o);
        std::cout << "emplace/reset ok\n";
    }

    std::cout << "=== 专节：optional vs expected ===\n";
    // optional：有/无，无原因
    // expected：值或带原因的错误；调用方想知道“为什么没有”时升级到 expected
    // ⚠️ *o 在空时是 UB —— 先 if(o) 或 value()/value_or
    std::cout << "optional = presence only; upgrade to expected for reasons\n";

    std::cout << "[std_optional_recap] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section05/std_optional_recap", run>;

}  // namespace

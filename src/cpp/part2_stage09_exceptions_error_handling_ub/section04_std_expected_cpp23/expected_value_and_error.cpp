// Topic    : expected 取值/取错：*e vs value()，error()，bad_expected_access
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 4.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_value_and_error
// Topic id : part2/stage09/section04/expected_value_and_error
// Refs     : https://en.cppreference.com/w/cpp/utility/expected
//            https://en.cppreference.com/w/cpp/utility/expected/bad_expected_access

#include "learn/topic_registry.hpp"

#include <cassert>
#include <expected>
#include <iostream>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [expected_value_and_error] 主干：安全访问 ===\n";
    {
        std::expected<int, std::string> ok = 10;
        assert(ok.has_value());
        assert(*ok == 10);
        assert(ok.value() == 10);

        std::expected<int, std::string> err = std::unexpected(std::string{"nope"});
        assert(!err);
        assert(err.error() == "nope");
        std::cout << "ok.value()=" << ok.value() << " err.error()=" << err.error() << '\n';
    }

    std::cout << "=== 对抗：value() 在错误时抛 bad_expected_access ===\n";
    {
        std::expected<int, std::string> err = std::unexpected(std::string{"E"});
        [[maybe_unused]] bool threw = false;
        try {
            (void)err.value();
            assert(false);
        } catch (const std::bad_expected_access<std::string>& e) {
            threw = true;
            assert(e.error() == "E");
            std::cout << "bad_expected_access.error()=" << e.error() << '\n';
        }
        assert(threw);
    }

    std::cout << "=== 专节：*e / e-> 不检查（错误时是 UB）===\n";
    // 不确定时：先 if (e) 或用 value() / value_or。
    // 本 topic 故意不在错误状态解引用 *e，避免 UB。
    {
        std::expected<int, std::string> ok = 3;
        if (ok) {
            assert(*ok == 3);
        }
        // 危险反例（勿运行）：
        // std::expected<int,std::string> bad = std::unexpected("x");
        // int v = *bad; // UB when !has_value()
        std::cout << "use if(e) / value() / value_or; never *e when empty\n";
    }

    // operator-> 同样要求有值
    {
        struct S {
            int n = 5;
        };
        std::expected<S, int> e = S{7};
        assert(e->n == 7);
        std::cout << "operator-> when has_value: " << e->n << '\n';
    }

    std::cout << "[expected_value_and_error] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_value_and_error", run>;

}  // namespace

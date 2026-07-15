// Topic    : if 语句（条件分支基础 + 作用域边界）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 1
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : if_statement
// Topic id : part2/stage08/section01/if_statement
// Refs     : https://en.cppreference.com/w/cpp/language/if
//            ISO [stmt.if]；现代增强见 section02（init-statement / if constexpr 定位）

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

// 返回值同时携带“是否成功 + 载荷”，方便展示 if/else 决策。
struct LookupResult {
    bool found{};
    int value{};
};

LookupResult lookup_score(const std::string& name) {
    if (name == "alice") {
        return {true, 95};
    }
    if (name == "bob") {
        return {true, 88};
    }
    return {false, 0};
}

int grade_band(int score) {
    // 经典 if / else if / else 链：自上而下短路求值，只进第一个真分支。
    if (score >= 90) {
        return 4;
    } else if (score >= 80) {
        return 3;
    } else if (score >= 70) {
        return 2;
    } else if (score >= 60) {
        return 1;
    } else {
        return 0;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [if_statement] 入门：最小正确用法 ===\n";
    {
        const int temp = 18;
        std::string weather;
        if (temp > 30) {
            weather = "hot";
        } else if (temp >= 15) {
            weather = "mild";
        } else {
            weather = "cold";
        }
        assert(weather == "mild");
        std::cout << "temp=" << temp << " -> " << weather << '\n';
    }

    std::cout << "=== 进阶：条件声明、短路与嵌套 ===\n";
    {
        // if (init 以外的声明形式) 条件本身也可以是声明（C++98 起）：
        // 变量作用域仅覆盖 if 体与 else 体，不泄漏到外层。
        if (int x = 7; x % 2 == 1) {  // C++17 init-statement 小样，细节见 section02
            assert(x == 7);
            std::cout << "odd branch: x=" << x << '\n';
        } else {
            assert(false && "should not reach even branch");
        }

        // 短路：左侧已足以决定真值时，右侧不求值。
        int calls = 0;
        auto bump = [&]() -> bool {
            ++calls;
            return true;
        };
        if (false && bump()) {
            assert(false);
        }
        assert(calls == 0);  // && 左假 → 右不调

        if (true || bump()) {
            assert(calls == 0);  // || 左真 → 右不调
        }

        // 嵌套 if：内层 else 绑定最近 if（“悬挂 else”规则）。
        int a = 1, b = 0, path = 0;
        if (a) {
            if (b) {
                path = 1;
            } else {
                path = 2;  // 绑定内层 if (b)
            }
        } else {
            path = 3;
        }
        assert(path == 2);
        std::cout << "dangling-else path=" << path << " (inner else)\n";

        // 用函数返回值做决策，避免“临时 bool 被丢弃后才想用载荷”。
        if (auto r = lookup_score("alice"); r.found) {
            assert(r.value == 95);
            std::cout << "alice score=" << r.value << '\n';
        } else {
            assert(false);
        }
        if (auto r = lookup_score("carol"); !r.found) {
            std::cout << "carol not found (as expected)\n";
        } else {
            assert(false);
        }
    }

    std::cout << "=== 专家：布尔语境转换、悬空 else、与现代 if 家族边界 ===\n";
    {
        // 条件在布尔语境中转换：指针/可选“有值”语义常依赖此规则。
        const char* p = "ok";
        if (p) {
            assert(p[0] == 'o');
        }
        p = nullptr;
        if (!p) {
            std::cout << "null pointer is false in condition\n";
        }

        // if constexpr / if consteval 属于编译期家族（阶段 6/7），
        // 本文件只做定位：它们不是运行期 if (init; cond) 的变体拼写。
        // - if constexpr：丢弃假分支（模板上下文）
        // - if consteval：判断“当前是否立即求值上下文”
        // - if (init; cond)：运行期，init 作用域限定在 if/else
        assert(grade_band(95) == 4);
        assert(grade_band(85) == 3);
        assert(grade_band(55) == 0);
        std::cout << "grade_band chain ok\n";

        // 工程取舍：超长 else-if 链可读性差时，优先 switch（枚举/整型）
        // 或 map/visitor（开放类型集）；if 链适合“有序阈值 + 少量分支”。
        std::cout << "prefer switch/visit when branches grow large\n";
    }

    std::cout << "[if_statement] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/if_statement", run>;

}  // namespace

// Topic    : 带初始化器的 if / switch（C++17）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 4
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : if_switch_with_initializer_cpp17
// Topic id : part2/stage08/section02/if_switch_with_initializer_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/if
//            https://en.cppreference.com/w/cpp/language/switch
//            提案 P0305；ISO [stmt.if] [stmt.switch]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>

namespace {

std::optional<int> parse_positive(const std::string& s) {
    if (s.empty() || s[0] == '-') {
        return std::nullopt;
    }
    int v = 0;
    for (char ch : s) {
        if (ch < '0' || ch > '9') {
            return std::nullopt;
        }
        v = v * 10 + (ch - '0');
    }
    return v;
}

enum class Op : int { Add = 1, Mul = 2, Unknown = 0 };

Op parse_op(char c) {
    switch (c) {
        case '+':
            return Op::Add;
        case '*':
            return Op::Mul;
        default:
            return Op::Unknown;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [if_switch_with_initializer_cpp17] 入门：if (init; cond) ===\n";
    {
        std::map<std::string, int> m{{"a", 1}, {"b", 2}};

        // it 只在 if/else 内可见，不污染外层。
        if (auto it = m.find("a"); it != m.end()) {
            assert(it->second == 1);
            std::cout << "found a=" << it->second << '\n';
        } else {
            assert(false);
        }

        if (auto it = m.find("z"); it != m.end()) {
            assert(false);
        } else {
            std::cout << "z missing (else branch)\n";
        }
        // 此处 it 不可见
    }

    std::cout << "=== 进阶：结构化绑定 + switch 初始化器 ===\n";
    {
        // ⭐ 黄金搭档：insert 返回 pair<iterator,bool>
        std::set<std::string> s{"hello"};
        if (auto [iter, success] = s.insert("world"); success) {
            assert(*iter == "world");
            std::cout << "inserted " << *iter << '\n';
        } else {
            assert(false);
        }
        if (auto [iter, success] = s.insert("hello"); !success) {
            assert(*iter == "hello");
            std::cout << "duplicate " << *iter << '\n';
        } else {
            assert(false);
        }

        // optional 载荷：init 声明 + 条件用 * 或显式判断
        if (auto opt = parse_positive("42"); opt.has_value()) {
            assert(*opt == 42);
            std::cout << "parsed " << *opt << '\n';
        }
        if (auto opt = parse_positive("-3"); !opt) {
            std::cout << "reject negative\n";
        }

        // switch (init; cond)：init 与 case 同作用域族
        const char symbol = '*';
        switch (Op op = parse_op(symbol); op) {
            case Op::Add:
                assert(false);
                break;
            case Op::Mul:
                std::cout << "op is Mul\n";
                break;
            default:
                assert(false);
                break;
        }

        // 频率统计：串起 init-if + 结构化绑定
        std::map<char, int> freq;
        const std::string text = "banana";
        for (char ch : text) {
            if (auto [it, ok] = freq.try_emplace(ch, 1); !ok) {
                ++(it->second);
            }
        }
        assert(freq['b'] == 1);
        assert(freq['a'] == 3);
        assert(freq['n'] == 2);
        std::cout << "freq: a=" << freq['a'] << " n=" << freq['n'] << '\n';
    }

    std::cout << "=== 专家：作用域模型、与 if constexpr/consteval 正交 ===\n";
    {
        // if (init; cond) 等价于 { init; if (cond) ... else ... }
        // 但 init 与 cond 声明的名字在同一作用域，覆盖 then/else。
        [[maybe_unused]] int outer = 1;
        if (int outer = 2; outer > 0) {  // 遮蔽外层 outer
            assert(outer == 2);
        }
        assert(outer == 1);

        // 正交三分：
        // 1) if (init; cond)     — 运行期，限域临时
        // 2) if constexpr (c)    — 编译期丢弃假分支（阶段 6）
        // 3) if consteval        — 立即求值上下文（阶段 7）
        // 可组合：if constexpr 内仍可写运行期 if (init; ...)。

        // 工程收益：
        // - 避免“先声明迭代器再 if”的两段式泄漏
        // - 锁/句柄/查找结果与使用紧贴，减少 use-after-scope 心智负担
        // C++23：init 还可写 using 别名，见 section04/init_statement_alias_declaration
        std::cout << "init-statement scopes names to the selection statement\n";
    }

    std::cout << "[if_switch_with_initializer_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/if_switch_with_initializer_cpp17", run>;

}  // namespace

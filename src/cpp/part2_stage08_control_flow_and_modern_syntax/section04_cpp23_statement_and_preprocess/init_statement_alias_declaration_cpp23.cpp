// Topic    : init-statement 允许别名声明（P2360, C++23）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 9.4
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : init_statement_alias_declaration_cpp23
// Topic id : part2/stage08/section04/init_statement_alias_declaration_cpp23
// Refs     : https://en.cppreference.com/w/cpp/language/if
//            提案 P2360；ISO init-statement 可含 alias-declaration

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace {

std::vector<std::pair<std::string, int>> make_table() {
    return {{"a", 1}, {"b", 2}, {"c", 3}};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [init_statement_alias_declaration_cpp23] 入门 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        int sum = 0;
        // C++23：范围 for 的 init-statement 可以是 using 别名声明
        for (using T = int; T e : v) {
            sum += e;
        }
        assert(sum == 6);
        std::cout << "for (using T = int; T e : v) sum=" << sum << '\n';
    }

    std::cout << "=== 进阶：if / switch 中的 using + 缩短长类型 ===\n";
    {
        std::map<std::string, int> m{{"x", 10}};

        // init-statement 只能有一条；using 作 init，condition 为表达式
        if (using Key = std::string; m.find(Key{"x"}) != m.end()) {
            assert(m.at("x") == 10);
            std::cout << "if (using Key = string; m.find(Key{...}) != end) ok\n";
        }

        const int code = 2;
        // init = alias-declaration；condition = 表达式（以 Status 收窄语义）
        switch (using Status = int; static_cast<Status>(code)) {
            case 1:
                assert(false);
                break;
            case 2:
                std::cout << "switch (using Status = int; ...) ok\n";
                break;
            default:
                assert(false);
                break;
        }

        // 长迭代器类型就地起别名，避免污染外层命名空间
        [[maybe_unused]] int total = 0;
        for (using Row = std::pair<std::string, int>; const Row& row : make_table()) {
            total += row.second;
        }
        assert(total == 6);
    }

    std::cout << "=== 专家：init-statement 允许的形式演变 ===\n";
    {
        // init-statement 历史：
        // - C++17：if/switch 引入 init；expression-statement 或 simple-declaration
        // - C++20：范围 for 也支持 init
        // - C++23 P2360：增加 alias-declaration（using / template using）
        //
        // 别名作用域：与 init 声明的变量相同，覆盖整个控制结构，结束后不可见。
        //
        // 工程价值：泛型代码里“只在这一支需要的类型别名”不必提到函数作用域，
        // 减少名字碰撞；也可让 structured binding / 迭代器类型更可读。

        std::vector<int> v{4, 5};
        [[maybe_unused]] int prod = 1;
        for (using Elem = int; Elem x : v) {
            prod *= x;
        }
        assert(prod == 20);

        // 仍可用经典变量 init；别名是增量能力，不是替代。
        for (auto data = make_table(); const auto& [k, val] : data) {
            (void)k;
            assert(val >= 1);
        }
        std::cout << "P2360: alias-declaration joins expression/simple-decl forms\n";
    }

    std::cout << "[init_statement_alias_declaration_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section04/init_statement_alias_declaration_cpp23", run>;

}  // namespace

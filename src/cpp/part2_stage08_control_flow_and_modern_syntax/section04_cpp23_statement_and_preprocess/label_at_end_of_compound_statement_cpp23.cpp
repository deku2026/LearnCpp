// Topic    : 标签可位于复合语句末尾（P2324, C++23）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 9.3
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : label_at_end_of_compound_statement_cpp23
// Topic id : part2/stage08/section04/label_at_end_of_compound_statement_cpp23
// Refs     : https://en.cppreference.com/w/cpp/language/statements
//            提案 P2324R2；ISO [stmt.label]

#include "learn/topic_registry.hpp"

#include <iostream>

namespace {

// C++23 前：END 后必须有语句（常写空语句 `;`）
[[maybe_unused]] int old_style_skip(int x) {
    if (x > 0) {
        goto END;
    }
    x = 42;
END:;  // 空语句占位
    return x;
}

// C++23：标签可直接出现在复合语句末尾
[[maybe_unused]] int new_style_skip(int x) {
    if (x > 0) {
        goto END;
    }
    x = 42;
END:  // 🆕 无需补 `;`
    return x;
}

[[maybe_unused]] int multi_label_exit(int mode) {
    int state = 0;
    {
        if (mode == 1) {
            state = 1;
            goto done;
        }
        if (mode == 2) {
            state = 2;
            goto done;
        }
        state = 3;
    done:  // 末尾标签：统一出口
    }
    return state;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [label_at_end_of_compound_statement_cpp23] 入门 ===\n";
    {
        assert(old_style_skip(5) == 5);   // 跳过赋值
        assert(old_style_skip(0) == 42);  // 执行赋值
        assert(new_style_skip(5) == 5);
        assert(new_style_skip(0) == 42);
        std::cout << "end label without null statement (C++23)\n";
    }

    std::cout << "=== 进阶：统一出口、与 case 标签区分 ===\n";
    {
        assert(multi_label_exit(1) == 1);
        assert(multi_label_exit(2) == 2);
        assert(multi_label_exit(0) == 3);
        std::cout << "multi-way goto to end label ok\n";

        // case/default 仍是 switch 的标签；本特性针对普通标识符标签。
        // 嵌套块末尾同样允许：
        [[maybe_unused]] int n = 0;
        {
            {
                if (true) {
                    goto inner_end;
                }
                n = -1;
            inner_end:
            }
            n += 10;
        }
        assert(n == 10);
    }

    std::cout << "=== 专家：C 兼容动机、与 goto 纪律 ===\n";
    {
        // 动机：C 允许标签出现在块末；C++ 历史上要求 label 后必须有 statement，
        // 导致从 C 粘贴代码或代码生成器要额外插 `;`。P2324 对齐 C。
        //
        // 语义：标签不引入作用域、不执行代码；只是跳转目标。
        // 仍遵守“不得跳过变量初始化”等既有 goto 规则。
        //
        // 工程：特性降低语法摩擦，不构成鼓励 goto 的理由；
        // RAII + return 仍是默认出口模型。

        [[maybe_unused]] int x = 1;
        {
            goto finish;
            x = 0;
        finish:
        }
        assert(x == 1);
        std::cout << "P2324 is C compatibility; still prefer structured exits\n";
    }

    std::cout << "[label_at_end_of_compound_statement_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage08/section04/label_at_end_of_compound_statement_cpp23", run>;

}  // namespace

// Topic    : goto 与标签（边界、合法场景、C++23 标签位置）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 1 / 9.3
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : goto_and_labels
// Topic id : part2/stage08/section01/goto_and_labels
// Refs     : https://en.cppreference.com/w/cpp/language/goto
//            https://en.cppreference.com/w/cpp/language/statements
//            ISO [stmt.goto] [stmt.label]；标签末尾见 section04

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

// 教学向：多层嵌套里“向前跳出”可用 goto 简化；
// 现代 C++ 更常靠 RAII + return / 提取函数 / 异常。
int search_grid(const int grid[2][3], int target, int& out_r, int& out_c) {
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (grid[r][c] == target) {
                out_r = r;
                out_c = c;
                goto found;
            }
        }
    }
    return 0;
found:
    return 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [goto_and_labels] 入门：标签与无条件跳转 ===\n";
    {
        int step = 0;
        goto skip_zero;
        step = -1;  // 被跳过
    skip_zero:
        step = 1;
        assert(step == 1);
        std::cout << "forward goto skipped dead assignment\n";

        // 向后跳形成循环（可读性差，仅作语义演示）。
        int n = 0;
    again:
        ++n;
        if (n < 3) {
            goto again;
        }
        assert(n == 3);
        std::cout << "backward goto loop n=" << n << '\n';
    }

    std::cout << "=== 进阶：多层跳出、错误路径汇合（C 风格） ===\n";
    {
        const int grid[2][3] = {{1, 2, 3}, {4, 5, 6}};
        int r = -1, c = -1;
        const int ok = search_grid(grid, 5, r, c);
        assert(ok == 1 && r == 1 && c == 1);
        std::cout << "found 5 at (" << r << "," << c << ")\n";

        r = -1;
        c = -1;
        assert(search_grid(grid, 99, r, c) == 0);

        // C 风格资源清理：多出口汇到同一 label（C++ 优先 RAII）。
        int resource = 0;
        int status = 0;
        {
            resource = 1;  // acquire
            const bool fail_mid = true;
            if (fail_mid) {
                status = -1;
                goto cleanup;
            }
            status = 1;
        cleanup:
            resource = 0;  // release
        }
        assert(status == -1 && resource == 0);
        std::cout << "C-style cleanup via goto (prefer RAII in C++)\n";
    }

    std::cout << "=== 专家：不可跨越初始化、跨函数禁止、C++23 标签末尾 ===\n";
    {
        // 规则：goto 不能从变量作用域外跳进“会跳过初始化”的位置。
        // 下面合法：跳到声明之前，或跳到已结束作用域之后。
        int path = 0;
        {
            if (true) {
                goto after_block;
            }
            int only_here = 42;
            path = only_here;
        }
    after_block:
        assert(path == 0);
        std::cout << "goto out of block is fine; into init is not\n";

        // 非法示例（注释，勿编译）:
        // goto into;
        // { int x = 1; into: use(x); }  // 跳过了 x 的初始化 → 病式

        // goto 只能跳到同一函数内的标签；不能跨函数。
        // 计算 goto（C 的 void* 标签地址）不是标准 C++。

        // C++23 P2324：标签可以出现在复合语句末尾，无需再写空语句 `;`。
        // 详见 section04/label_at_end_of_compound_statement_cpp23。
        int x = 0;
        {
            if (x == 0) {
                goto end_label;
            }
            x = 99;
        end_label:  // C++23：末尾标签 OK
        }
        assert(x == 0);
        std::cout << "C++23: label may end a compound statement\n";

        // 工程纪律：日常业务逻辑避免 goto；可接受的残留场景主要是
        // 代码生成、与 C API 错误路径互操作、极少数打破深层嵌套的可读出口。
        // 结构化替代：提取函数 + return、RAII、布尔标志、异常。
        std::cout << "prefer structured control flow over goto\n";
    }

    std::cout << "[goto_and_labels] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/goto_and_labels", run>;

}  // namespace

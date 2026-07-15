// Topic    : [[assume]]（C++23）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 8
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : assume_attribute_cpp23
// Topic id : part2/stage08/section03/assume_attribute_cpp23
// Refs     : https://en.cppreference.com/w/cpp/language/attributes/assume
//            提案 P1774R8；ISO [dcl.attr.assume]
// ⚠️ 假设为假即 UB —— 本文件只在断言保证成立的输入上调用

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

// 调用方保证 n > 0。assume 让优化器去掉“n 可能为 0”的顾虑（实现相关）。
int reciprocal_percent(int n) {
    [[assume(n > 0)]];
    return 100 / n;
}

// 调用方保证 n % 4 == 0 且 n >= 0。
void scale_by_two(int* data, int n) {
    [[assume(n >= 0)]];
    [[assume(n % 4 == 0)]];
    for (int i = 0; i < n; ++i) {
        data[i] *= 2;
    }
}

int sum_positive_length(const std::vector<int>& v) {
    const int n = static_cast<int>(v.size());
    [[assume(n >= 0)]];  // size 转 int 后仍非负（向量不过大时）
    int s = 0;
    for (int i = 0; i < n; ++i) {
        s += v[static_cast<std::size_t>(i)];
    }
    return s;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [assume_attribute_cpp23] 入门：向优化器断言恒真 ===\n";
    {
        assert(reciprocal_percent(4) == 25);
        assert(reciprocal_percent(1) == 100);
        std::cout << "reciprocal_percent(4)=" << reciprocal_percent(4) << '\n';

        std::vector<int> buf{1, 2, 3, 4, 5, 6, 7, 8};
        scale_by_two(buf.data(), static_cast<int>(buf.size()));
        assert((buf == std::vector<int>{2, 4, 6, 8, 10, 12, 14, 16}));
        std::cout << "scale_by_two with n%4==0 ok\n";
    }

    std::cout << "=== 进阶：表达式不求值（无副作用） ===\n";
    {
        // cppreference：assume 的表达式被语境转换为 bool，但不求值。
        int x = 5;
        [[assume(x > 0)]];
        assert(x == 5);

        // 若写成 [[assume(x++ > 0)]]，递增副作用不会发生（且实现可警告）。
        // 因此 assume 内禁止依赖副作用；需要检查请用 assert/if。
        int y = 10;
        [[assume(y % 2 == 0)]];
        assert(y == 10);
        std::cout << "assume does not evaluate expression side effects\n";

        assert(sum_positive_length(std::vector<int>{1, 2, 3}) == 6);
    }

    std::cout << "=== 专家：假则 UB、与 assert 对照、godbolt 观察 ===\n";
    {
        // ⚠️⚠️ 若运行时假设为假（返回 false / 抛异常 / 本身 UB），
        // 整个程序行为未定义。编译器可删除“假设为假才需要的代码”。
        //
        // 本教学路径：所有 assume 前用 assert 保证条件，不演示假路径。
        int n = 8;
        assert(n > 0 && n % 4 == 0);
        std::vector<int> data(static_cast<std::size_t>(n), 1);
        scale_by_two(data.data(), n);
        assert(std::accumulate(data.begin(), data.end(), 0) == 16);

        // assert：调试期检查，失败可诊断；Release 可被 NDEBUG 去掉。
        // assume：始终向优化器承诺，失败即 UB，不是运行期检查。
        // 能写 if/assert 保护的，不要换成 assume 图省事。

        // 历史扩展：__builtin_assume（Clang）、__assume（MSVC）；
        // C++23 标准化为 [[assume(expr)]]。
        // godbolt：-O2 -std=c++23 对比有无 assume，观察向量化/去分支差异。

        // 与 [[likely]]：likely 是概率；assume 是逻辑必然。
        std::cout << "false assumption => UB; prefer assert for checks\n";
        std::cout << "use assume only when invariant is proven\n";
    }

    std::cout << "[assume_attribute_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/assume_attribute_cpp23", run>;

}  // namespace

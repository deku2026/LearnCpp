// Topic     : C++23 字面值后缀 z / uz（signed size_t / size_t）
// Doc       : 第2部分-阶段2 · 步骤 1.4
// cppreference: https://en.cppreference.com/cpp/language/integer_literal
// 提案     : P0330R8
//
// 要点: 0uz → std::size_t，与 container::size() 同型，消灭 -Wsign-compare；
//       0z → 有符号对应（通常 std::ptrdiff_t），适合可能为负的索引差。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [size_t_ptrdiff_literal_suffix_cpp23] ===\n";

    std::vector<int> v{10, 20, 30, 40};

    // -------------------------------------------------------------------------
    // 入门：问题是什么
    // -------------------------------------------------------------------------
    // for (int i = 0; i < v.size(); ++i)  // int vs size_t → 有符号比较警告/隐患
    // 经典修复：std::size_t i = 0;  或  static_cast / ssize（C++20）

    // -------------------------------------------------------------------------
    // 进阶：C++23 后缀
    // -------------------------------------------------------------------------
#if defined(__cpp_size_t_suffix) && (__cpp_size_t_suffix >= 202011L)
    static_assert(std::is_same_v<decltype(0uz), std::size_t>);
    // 0z 的类型是 std::make_signed_t<std::size_t>，通常即 ptrdiff_t
    static_assert(std::is_same_v<decltype(0z), std::make_signed_t<std::size_t>>);
    static_assert(std::is_same_v<decltype(0z), std::ptrdiff_t>);

    std::size_t sum_u = 0;
    for (auto i = 0uz; i < v.size(); ++i) {
        sum_u += static_cast<std::size_t>(v[i]);
        assert(v[i] == 10 * static_cast<int>(i + 1));
    }
    assert(sum_u == 100);
    std::cout << "[intro] 0uz loop sum=" << sum_u << " (same type as v.size())\n";

    // 需要有符号索引时（倒序、差值）
    auto last = static_cast<std::ptrdiff_t>(v.size()) - 1z;
    assert(last == 3);
    int rev_sum = 0;
    for (auto i = last; i >= 0z; --i) {
        rev_sum += v[static_cast<std::size_t>(i)];
    }
    assert(rev_sum == 100);
    std::cout << "[advanced] 0z / ptrdiff-style reverse sum=" << rev_sum << '\n';

    // 大写后缀等价
    static_assert(std::is_same_v<decltype(1UZ), std::size_t>);
    static_assert(std::is_same_v<decltype(1Z), std::ptrdiff_t>);

#else
    // 工具链尚未提供后缀时：显式 size_t / ptrdiff_t 达到同等教学目的
    std::size_t sum_u = 0;
    for (std::size_t i = 0; i < v.size(); ++i) {
        sum_u += static_cast<std::size_t>(v[i]);
    }
    assert(sum_u == 100);
    std::cout << "[intro] __cpp_size_t_suffix missing; used explicit size_t\n";

    auto last = static_cast<std::ptrdiff_t>(v.size()) - 1;
    int rev_sum = 0;
    for (std::ptrdiff_t i = last; i >= 0; --i) {
        rev_sum += v[static_cast<std::size_t>(i)];
    }
    assert(rev_sum == 100);
    std::cout << "[advanced] ptrdiff_t reverse sum=" << rev_sum << '\n';
#endif

    // -------------------------------------------------------------------------
    // 专家：与 ssize、ptrdiff 关系
    // -------------------------------------------------------------------------
    // C++20 std::ssize(c) 返回有符号，适合 i < ssize(v)
    // C++23 字面值后缀让“字面量本身”带上正确类型，少写 cast
    // 注意：size_t 无符号减法下溢（0uz - 1uz 巨大），倒序循环优先有符号或 do-while 技巧

    const auto n = v.size();
    assert(n > 0);
    // 危险示范（不执行）: for (auto i = n - 1; i >= 0; --i) 若 i 为 size_t，永不停
    std::cout << "[expert] unsigned countdown is a classic infinite-loop trap\n";
    std::cout << "=== size_t_ptrdiff_literal_suffix_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/size_t_ptrdiff_literal_suffix_cpp23", run>;

}  // namespace

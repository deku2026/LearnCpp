// Topic     : ranges::shift_left / shift_right (C++23)
// Doc       : 第2部分-阶段10 · 步骤 13.2
// cppreference: https://en.cppreference.com/cpp/algorithm/ranges/shift_left
//
// 要点: 元素搬移; 返回仍有效的子范围; 移出端值未指定。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ranges_shift_left_right_cpp23] ===\n";

    // C++20 已有 std::shift_left/right; ranges 版为 C++23
    // 部分 MSVC STL 尚未暴露 ranges::shift_*，用 std:: 等价回退
#if defined(__cpp_lib_shift)  // std::shift_* available since C++20
    std::vector<int> v{1, 2, 3, 4, 5};
    auto left_end = std::shift_left(v.begin(), v.end(), 2);
    // 有效前缀 [3,4,5]; 末 2 未指定
    assert(std::ranges::equal(std::ranges::subrange(v.begin(), left_end), std::vector{3, 4, 5}));
    assert(v[0] == 3 && v[1] == 4 && v[2] == 5);

    std::vector<int> w{1, 2, 3, 4, 5};
    auto right_begin = std::shift_right(w.begin(), w.end(), 2);
    // 有效后缀 [1,2,3]
    assert(std::ranges::equal(std::ranges::subrange(right_begin, w.end()), std::vector{1, 2, 3}));
    assert(w[2] == 1 && w[3] == 2 && w[4] == 3);

    // n == 0 / n >= size
    std::vector<int> a{1, 2, 3};
    auto r0 = std::shift_left(a.begin(), a.end(), 0);
    assert(std::ranges::equal(std::ranges::subrange(a.begin(), r0), a));
    std::vector<int> b{1, 2, 3};
    auto rbig = std::shift_left(b.begin(), b.end(), 10);
    assert(rbig == b.begin());  // 全移出 → 有效范围为空

    std::cout << "[shift_left/right] std::shift_* demo OK";
#if defined(__cpp_lib_ranges) /* ranges::shift_* may or may not exist */
    // 教学意图：标准接口名是 ranges::shift_left/right (C++23)
    std::cout << " (ranges::shift_* may still be missing on this STL)";
#endif
    std::cout << '\n';
#else
    std::cout << "library not yet available, manual rotate-style fallback\n";
    std::vector<int> v{1, 2, 3, 4, 5};
    std::rotate(v.begin(), v.begin() + 2, v.end());
    assert(v[0] == 3 && v[1] == 4 && v[2] == 5);
#endif

    std::cout << "ranges_shift_left_right_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_shift_left_right_cpp23", run>;

}  // namespace

// Topic    : C++23 constexpr unique_ptr（P2273）
// Doc      : 第2部分-阶段5 · 步骤 9.6
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : constexpr_unique_ptr_cpp23
// Topic id : part2/stage05/section04/constexpr_unique_ptr_cpp23
// Refs     : https://en.cppreference.com/w/cpp/memory/unique_ptr
//            P2273；feature-test __cpp_lib_constexpr_memory

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <version>

namespace {

#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L

constexpr int compute_with_unique_ptr() {
    auto p = std::make_unique<int>(42);
    return *p;
}

constexpr int sum_buffer() {
    auto buf = std::make_unique<int[]>(4);
    buf[0] = 1;
    buf[1] = 2;
    buf[2] = 3;
    buf[3] = 4;
    int s = 0;
    for (int i = 0; i < 4; ++i) {
        s += buf[i];
    }
    return s;
}

// 编译期移动 unique_ptr
constexpr int move_and_read() {
    auto a = std::make_unique<int>(7);
    auto b = std::move(a);
    // a 在 constexpr 中应为空；读 b
    return b ? *b : -1;
}

static_assert(compute_with_unique_ptr() == 42);
static_assert(sum_buffer() == 10);
static_assert(move_and_read() == 7);

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [constexpr_unique_ptr_cpp23] 入门 ===\n";
    std::cout << "__cpp_lib_constexpr_memory=" << __cpp_lib_constexpr_memory << '\n';

    constexpr int v = compute_with_unique_ptr();
    assert(v == 42);
    std::cout << "constexpr compute_with_unique_ptr=" << v << '\n';

    [[maybe_unused]] constexpr int s = sum_buffer();
    assert(s == 10);

    // 运行期同样可用
    auto p = std::make_unique<int>(100);
    assert(*p == 100);

    std::cout << "=== 进阶：为何重要 + 瞬态分配边界 ===\n";
    // · 编译期算法可用 RAII 指针风格，少写裸 new/delete
    // · 与阶段 7 constexpr 容器/算法衔接（P2273）
    // · default_delete::operator() 亦为 constexpr
    // · ⚠️ 与 constexpr vector 相同：分配必须在常量求值内完成析构（transient）
    //   不能 constexpr 持久化 unique_ptr 到运行期可见的静态存储
    constexpr int moved = move_and_read();
    assert(moved == 7);
    std::cout << "move_and_read()=" << moved << " (ownership transfer is constexpr)\n";

    std::cout << "=== 专家：feature-test / 与 shared_ptr 对照 ===\n";
    // · __cpp_lib_constexpr_memory >= 202202 检测库支持
    // · shared_ptr 的 constexpr 面更窄/更晚，别默认两者能力对称
    // · 运行期与编译期同一套所有权语义：移动后源为空

    auto rt = std::make_unique<int>(11);
    auto rt2 = std::move(rt);
    assert(!rt && rt2 && *rt2 == 11);

    std::cout << "=== constexpr_unique_ptr_cpp23: OK ===\n";
    return 0;
}

#else

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [constexpr_unique_ptr_cpp23] feature not available ===\n";
    std::cout << "need __cpp_lib_constexpr_memory >= 202202\n";
    // 运行期退化演示
    auto p = std::make_unique<int>(42);
    assert(p && *p == 42);
    return 0;
}

#endif

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/constexpr_unique_ptr_cpp23", run>;

}  // namespace

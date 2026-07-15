// Topic     : 顶层 const vs 底层 const
// Doc       : 第2部分-阶段2 · 步骤 3.1
// cppreference: https://en.cppreference.com/cpp/language/cv
//               https://en.cppreference.com/cpp/language/auto
//
// 要点: 顶层 const = 对象自身不可改；底层 = 指向/引用的对象不可改；
//       拷贝/按值 auto 忽略顶层 const，保留底层 const。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [top_level_vs_low_level_const] ===\n";

    // -------------------------------------------------------------------------
    // 入门：术语对齐
    // -------------------------------------------------------------------------
    int x = 1;
    int y = 2;

    // 顶层：指针本身 const
    int* const top = &x;
    *top = 10;  // OK
    // top = &y; // ❌
    assert(x == 10);

    // 底层：指向 const int
    [[maybe_unused]] const int* low = &x;
    low = &y;  // OK
    // *low = 3; // ❌
    assert(*low == 2);

    // 读法：从右往左 — int* const 是 const 指针；const int* 是指向 const 的指针
    [[maybe_unused]] const int z = 5;  // z 的 const 是顶层（对 z 这个对象）
    // z = 6; // ❌
    std::cout << "[intro] top-level: object itself; low-level: pointee/referee\n";

    // -------------------------------------------------------------------------
    // 进阶：拷贝与指针赋值规则
    // -------------------------------------------------------------------------
    const int ca = 1;
    [[maybe_unused]] int plain = ca;  // OK：拷贝时忽略源的顶层 const
    assert(plain == 1);

    const int* pc = &ca;
    // int* p = pc; // ❌ 不能丢底层 const（否则可通过 p 改 const 对象）
    [[maybe_unused]] const int* pc2 = pc;  // OK
    assert(*pc2 == 1);

    int* const cptr = &x;
    [[maybe_unused]] int* p2 = cptr;  // OK：拷贝指针值，目标不是 const 指针（顶层可丢）
    assert(p2 == &x);

    // 引用：const int& 是“底层”意义上的只读绑定
    [[maybe_unused]] const int& r = ca;
    // int& r2 = ca; // ❌
    assert(r == 1);
    std::cout << "[advanced] cannot drop low-level const when copying pointers\n";

    // -------------------------------------------------------------------------
    // 专家：auto / 模板按值推导只剥顶层
    // -------------------------------------------------------------------------
    auto a1 = ca;  // int，顶层 const 剥掉
    static_assert(std::is_same_v<decltype(a1), int>);

    auto a2 = pc;  // const int*，底层保留
    static_assert(std::is_same_v<decltype(a2), const int*>);

    auto a3 = cptr;  // int*，指针顶层 const 剥掉
    static_assert(std::is_same_v<decltype(a3), int*>);

    // 双重：const int* const
    const int* const both = &ca;
    auto a4 = both;  // const int* —— 只剥指针自身的顶层 const
    static_assert(std::is_same_v<decltype(a4), const int*>);

    // 类型特征辅助
    static_assert(std::is_const_v<const int>);
    static_assert(!std::is_const_v<const int*>);  // 指针类型本身不是 const
    static_assert(std::is_const_v<int* const>);
    static_assert(std::is_const_v<std::remove_pointer_t<const int*>>);

    std::cout << "[expert] auto/value params strip top-level const only\n";
    std::cout << "=== top_level_vs_low_level_const: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/top_level_vs_low_level_const", run>;

}  // namespace

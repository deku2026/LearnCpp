// Topic     : 指针基础
// Doc       : 第2部分-阶段2 · 步骤 4.3
// cppreference: https://en.cppreference.com/cpp/language/pointer
//               https://en.cppreference.com/cpp/language/operator_member_access
//
// 要点: 指针存地址，可空、可改绑；* 解引用 & 取址；
//       指针算术与数组关系；优先引用，需可空/改绑再用指针。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pointer_basics] ===\n";

    // -------------------------------------------------------------------------
    // 入门：取址、解引用、空指针
    // -------------------------------------------------------------------------
    int x = 42;
    int* p = &x;
    assert(p != nullptr);
    assert(*p == 42);
    *p = 7;
    assert(x == 7);

    int* np = nullptr;
    assert(np == nullptr);
    // *np; // UB —— 不解引用空指针

    // 指针可以重新指向
    int y = 100;
    p = &y;
    assert(*p == 100);
    std::cout << "[intro] pointer reseated from x to y\n";

    // -------------------------------------------------------------------------
    // 进阶：const 指针、void*、数组退化
    // -------------------------------------------------------------------------
    const int cx = 5;
    const int* pc = &cx;
    assert(*pc == 5);

    void* raw = &y;
    int* back = static_cast<int*>(raw);  // void* 需显式转回
    assert(back == &y);

    int arr[3]{10, 20, 30};
    int* pa = arr;  // 数组 → 首元素指针
    assert(*pa == 10);
    assert(*(pa + 1) == 20);
    assert(pa[2] == 30);
    assert(&pa[2] == arr + 2);

    // sizeof 数组 vs 指针
    assert(sizeof(arr) == 3 * sizeof(int));
    assert(sizeof(pa) == sizeof(int*));
    std::cout << "[advanced] array decays to pointer; arithmetic in elements\n";

    // -------------------------------------------------------------------------
    // 专家：多级指针、函数指针、与引用对比
    // -------------------------------------------------------------------------
    int* px = &x;
    int** pp = &px;
    assert(**pp == x);
    assert(*pp == px);
    **pp = 11;
    assert(x == 11);
    assert(**pp == 11);

    auto add1 = [](int n) { return n + 1; };
    // 函数指针（非捕获 lambda 可转）
    int (*fp)(int) = add1;
    assert(fp(3) == 4);

    // 指针 vs 引用（心智）
    // - 引用必须初始化、不可空、不可改绑（语法上像别名）
    // - 指针表达“可选 / 可重置 / 算术 / 多态基类拥有”
    int& ref = x;
    assert(&ref == &x);

    // 指向数组的指针（不退化）
    int (*parr)[3] = &arr;
    assert((*parr)[1] == 20);

    static_assert(std::is_pointer_v<int*>);
    static_assert(std::is_same_v<std::remove_pointer_t<int*>, int>);

    std::cout << "[expert] prefer references in APIs; pointers for optionality/arithmetic\n";
    std::cout << "=== pointer_basics: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/pointer_basics", run>;

}  // namespace

// Topic     : 声明解析（从右往左 / 螺旋法则入门）
// Doc       : 第2部分-阶段2 · 步骤 4（复合类型声明）
// cppreference: https://en.cppreference.com/cpp/language/declarations
//               https://en.cppreference.com/cpp/language/pointer
//               https://en.cppreference.com/cpp/language/reference
//               https://en.cppreference.com/cpp/language/function
//
// 要点: 声明 = 类型说明符 + 声明符；* & [] () 结合优先级；
//       用 using 别名拆解复杂声明；右值引用 && 与逻辑与无关。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>

namespace {

int add(int a, int b) {
    return a + b;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [declaration_parsing] ===\n";

    // -------------------------------------------------------------------------
    // 入门：从右往左读简单声明
    // -------------------------------------------------------------------------
    // int* p        → p 是指针，指向 int
    // int& r        → r 是引用，绑定 int
    // const int* p  → p 指向 const int
    // int* const p  → p 是 const 指针，指向 int

    int x = 1;
    int* p = &x;
    int& r = x;
    [[maybe_unused]] const int* pc = &x;
    [[maybe_unused]] int* const cp = &x;
    assert(*p == 1 && r == 1 && *pc == 1 && *cp == 1);
    static_assert(std::is_same_v<decltype(p), int*>);
    static_assert(std::is_same_v<decltype(r), int&>);
    std::cout << "[intro] read declarations inside-out / right-to-left\n";

    // -------------------------------------------------------------------------
    // 进阶：数组、函数、指针的优先级
    // -------------------------------------------------------------------------
    // [] 和 () 比 * 更紧
    int arr[3]{1, 2, 3};
    [[maybe_unused]] int* ptrs[3]{&arr[0], &arr[1], &arr[2]};  // 数组，元素是 int*
    int (*parr)[3] = &arr;                                     // 指向“3 个 int 的数组”的指针
    (void)parr;
    assert(*ptrs[1] == 2);
    assert((*parr)[2] == 3);

    int (*fp)(int, int) = &add;  // 函数指针
    (void)fp;
    assert(fp(2, 3) == 5);

    // 引用数组？数组的引用：
    int (&rarr)[3] = arr;
    rarr[0] = 9;
    assert(arr[0] == 9);

    // 易混：
    // int *a[3];     指针的数组
    // int (*a)[3];   数组的指针
    // int *f();      返回 int* 的函数
    // int (*f)();    函数指针
    std::cout << "[advanced] []/() bind tighter than *; use parens for pointer-to-array\n";

    // -------------------------------------------------------------------------
    // 专家：using 拆解、&&、const 放置
    // -------------------------------------------------------------------------
    using Int = int;
    using IntPtr = Int*;
    [[maybe_unused]] IntPtr _alias_probe = nullptr;
    using Func = int(int, int);
    using FuncPtr = Func*;
    [[maybe_unused]] FuncPtr fp2 = add;
    assert(fp2(4, 5) == 9);

    using Arr3 = int[3];
    [[maybe_unused]] Arr3& ref_to_arr = arr;
    assert(ref_to_arr[0] == 9);

    // C++11 起：右值引用
    [[maybe_unused]] int&& rr = 1;
    assert(rr == 1);

    // const 出现位置
    // const int *   == int const *   （指向 const）
    // int * const                 （const 指针）
    static_assert(std::is_same_v<const int*, const int*>);

    // 复杂示例可读性：别写 int*(*(*p)())[3]; —— 分层 using
    std::cout << "[expert] prefer using-aliases over nested C declarators\n";
    std::cout << "=== declaration_parsing: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/declaration_parsing", run>;

}  // namespace

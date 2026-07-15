// Topic     : const 限定：指针与引用
// Doc       : 第2部分-阶段2 · 步骤 3.1–3.2
// cppreference: https://en.cppreference.com/cpp/language/cv
//               https://en.cppreference.com/cpp/language/reference
//
// 要点: const 对象不可改；const T* 底层、T* const 顶层；
//       const T& 可绑右值并延长临时量寿命（“最重要的 const”）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

void print_string(const std::string& s) {
    // 既能收左值也能收右值，且不拷贝
    assert(!s.empty() || s.empty());
    std::cout << "[call] " << s << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [const_qualifier_pointer_reference] ===\n";

    // -------------------------------------------------------------------------
    // 入门：const 对象与引用
    // -------------------------------------------------------------------------
    const int c = 10;
    // c = 11; // ❌
    const int& cr = c;
    assert(cr == 10);

    int x = 1;
    const int& cx = x;  // 只读别名；x 本身仍可通过其它路径改
    x = 2;
    assert(cx == 2);
    // cx = 3; // ❌
    std::cout << "[intro] const object/ref not assignable through that name\n";

    // -------------------------------------------------------------------------
    // 进阶：指针两处 const + const& 绑右值
    // -------------------------------------------------------------------------
    int a = 10;
    int b = 20;

    const int* p1 = &a;  // 底层 const：不能改 *p1，能改 p1
    p1 = &b;
    // *p1 = 5; // ❌
    assert(*p1 == 20);

    int* const p2 = &a;  // 顶层 const：不能改 p2，能改 *p2
    *p2 = 15;
    // p2 = &b; // ❌
    assert(a == 15);

    const int* const p3 = &b;  // 双 const
    assert(*p3 == 20);
    // *p3 = 1; p3 = &a; // ❌

    // 最重要的 const：const T& 绑右值，延长临时寿命到引用作用域
    const int& r_lit = 42;
    assert(r_lit == 42);
    // int& bad = 42; // ❌

    std::string local = "left";
    print_string(local);
    print_string(std::string{"temp"});  // 临时 string，const& 绑定
    print_string("literal");            // 构造临时 string
    std::cout << "[advanced] const T& binds rvalues and extends temporary lifetime\n";

    // -------------------------------------------------------------------------
    // 专家：const 成员函数、去掉 const 的边界
    // -------------------------------------------------------------------------
    struct Counter {
        int n{0};
        int get() const { return n; }  // const 成员函数：不修改 *this 逻辑状态
        void inc() { ++n; }
    };
    const Counter cc{};
    assert(cc.get() == 0);
    // cc.inc(); // ❌

    // const_cast 仅在“对象本非 const”时用于去 const；对真 const 对象写入 = UB
    int mutable_obj = 5;
    const int* pm = &mutable_obj;
    *const_cast<int*>(pm) = 6;  // OK：底层对象非 const
    assert(mutable_obj == 6);

    std::cout << "[expert] const_cast is not a license to mutate truly const objects\n";
    std::cout << "=== const_qualifier_pointer_reference: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/const_qualifier_pointer_reference", run>;

}  // namespace

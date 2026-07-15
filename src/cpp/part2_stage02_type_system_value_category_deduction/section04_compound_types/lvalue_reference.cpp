// Topic     : 左值引用 T&
// Doc       : 第2部分-阶段2 · 步骤 4.1
// cppreference: https://en.cppreference.com/cpp/language/reference
//
// 要点: 左值引用是已有对象的别名；必须初始化；绑定后不可改绑；
//       非 const T& 只绑左值；const T& 可绑右值并延长临时寿命。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

void add_bang(std::string& s) {
    s += "!";
}

void show(const std::string& s) {
    std::cout << "[show] " << s << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [lvalue_reference] ===\n";

    // -------------------------------------------------------------------------
    // 入门：别名语义
    // -------------------------------------------------------------------------
    std::string s = "Example";
    std::string& r = s;
    r += "!";
    assert(s == "Example!");
    // std::string& bad; // ❌ 必须初始化

    int a = 1;
    int& ra = a;
    ra = 2;
    assert(a == 2);
    // “改绑”其实是给原对象赋值：
    int b = 3;
    ra = b;  // a 变成 3，ra 仍绑定 a
    assert(a == 3 && &ra == &a);
    std::cout << "[intro] reference is an alias; assignment writes through it\n";

    // -------------------------------------------------------------------------
    // 进阶：绑定规则与传参
    // -------------------------------------------------------------------------
    // int& rlit = 42; // ❌ 非 const 左值引用不能绑右值
    [[maybe_unused]] const int& cr = 42;  // OK，临时 int 寿命延长到 cr 作用域结束
    assert(cr == 42);

    add_bang(s);
    assert(s == "Example!!");
    show(s);
    show(std::string{"temp"});  // const& 收临时量
    show("literal");

    // 引用折叠预告：typedef int& L; L& x → int&
    using Lref = int&;
    Lref& rr = a;  // 仍是 int&
    static_assert(std::is_same_v<decltype(rr), int&>);
    std::cout << "[advanced] non-const & : lvalues only; const & : also rvalues\n";

    // -------------------------------------------------------------------------
    // 专家：引用成员、返回引用、悬空风险
    // -------------------------------------------------------------------------
    struct HoldsRef {
        int& r;
    };
    HoldsRef h{a};
    h.r = 9;
    assert(a == 9);

    auto& back = s;  // auto& 保留引用
    back += "?";
    assert(!s.empty() && s.back() == '?');

    // 危险：返回局部的引用 → 悬空（不要做）
    // int& dang() { int t = 1; return t; }

    // 范围 for 中的引用
    int arr[]{1, 2, 3};
    for (int& e : arr) {
        e *= 2;
    }
    assert(arr[0] == 2 && arr[2] == 6);

    std::cout << "[expert] refs don't extend lifetime of locals when returned\n";
    std::cout << "=== lvalue_reference: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/lvalue_reference", run>;

}  // namespace

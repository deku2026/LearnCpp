// Topic     : glvalue 与 rvalue 复合类别
// Doc       : 第2部分-阶段2 · 步骤 5.2 / 5.5
// cppreference: https://en.cppreference.com/cpp/language/value_category
//
// 要点: glvalue = lvalue ∪ xvalue（有身份）；rvalue = xvalue ∪ prvalue（可移动）；
//       属性：取地址、放赋值左边、绑定何种引用。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

struct Probe {
    int n{0};
    Probe& operator=(const Probe&) = default;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [glvalue_and_rvalue] ===\n";

    // -------------------------------------------------------------------------
    // 入门：复合类别图
    // -------------------------------------------------------------------------
    // expression
    //   ├─ glvalue (identity) ─┬─ lvalue
    //   │                      └─ xvalue ─┐
    //   └─ rvalue (movable) ──────────────┴─ prvalue
    // xvalue 同时属于 glvalue 与 rvalue。

    int i = 0;
    Probe p{};

    // glvalue：可问“是谁”（有对象身份）
    int* pi = &i;  // OK：lvalue 可取地址
    assert(pi == &i);
    // &42; // ❌ prvalue 无身份
    // &std::move(i); // ❌ xvalue 虽有身份但语言禁止对其取地址

    std::cout << "[intro] glvalue ≈ has identity; rvalue ≈ movable resource\n";

    // -------------------------------------------------------------------------
    // 进阶：绑定与赋值
    // -------------------------------------------------------------------------
    int& lr = i;  // 非 const 左值引用 ← 仅 lvalue
    // int& bad1 = 42;
    // int& bad2 = std::move(i);
    assert(&lr == &i);

    const int& cr1 = i;             // const& ← lvalue
    const int& cr2 = 42;            // const& ← prvalue（延长寿命）
    const int& cr3 = std::move(i);  // const& ← xvalue
    assert(cr1 == 0 && cr2 == 42);

    int&& rr1 = 42;            // && ← prvalue
    int&& rr2 = std::move(i);  // && ← xvalue
    // int&& bad = i;         // ❌ 不绑 lvalue
    assert(rr1 == 42);

    // 赋值号左边需要可修改的 glvalue（通常是非 const lvalue）
    i = 5;
    // 42 = i; // ❌
    p = Probe{};  // 左操作数 p 是 lvalue
    assert(i == 5);
    (void)cr3;
    (void)rr2;
    std::cout << "[advanced] ref binding table: & / const& / &&\n";

    // -------------------------------------------------------------------------
    // 专家：为何标准要复合类别
    // -------------------------------------------------------------------------
    // 许多规则说“需要 glvalue”（多态、成员访问的对象表达式历史模型等）
    // 或“需要 rvalue”（移动、某些重载）。
    // 成员访问：E1.E2 的值类别依赖 E1 与 E2 的性质（略，见 cppreference）。

    std::string s = "abc";
    auto&& fwd1 = s;                 // 左值 → string&
    auto&& fwd2 = std::string{"z"};  // prvalue → string&&
    static_assert(std::is_same_v<decltype(fwd1), std::string&>);
    static_assert(std::is_same_v<decltype(fwd2), std::string&&>);
    assert(fwd1 == "abc" && fwd2 == "z");

    // xvalue 是“有身份的可移动”：std::move(s) 仍指着 s 那块对象，但表示可偷
    std::string t = std::move(s);
    assert(t == "abc");

    std::cout << "[expert] xvalue is the overlap: identity + movable\n";
    std::cout << "=== glvalue_and_rvalue: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/glvalue_and_rvalue", run>;

}  // namespace

// Topic     : 默认构造函数
// Doc       : 第2部分-阶段4 · 步骤 3.1
// cppreference: https://en.cppreference.com/cpp/language/default_constructor
//
// 要点: 无参（或全默认实参）构造；用户声明任意构造会抑制隐式默认构造；
//       =default 显式要回默认构造；NSDMI 让默认构造有意义。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

struct ImplicitDefault {
    int x;
    // 无用户声明构造 → 隐式默认构造（对 int 是默认初始化，局部不置 0）
};

struct WithNsdmi {
    int x = 42;
    std::string s = "hi";
};

struct OnlyParamCtor {
    explicit OnlyParamCtor(int v) : v_(v) {}
    int v_;
};

struct RestoreDefault {
    int v_ = 0;
    RestoreDefault() = default;
    explicit RestoreDefault(int v) : v_(v) {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [default_constructor] ===\n";

    // -------------------------------------------------------------------------
    // §入门：隐式默认 + NSDMI
    // -------------------------------------------------------------------------
    static_assert(std::is_default_constructible_v<WithNsdmi>);
    WithNsdmi a;
    assert(a.x == 42 && a.s == "hi");
    std::cout << "[intro] WithNsdmi x=" << a.x << " s=" << a.s << '\n';

    // -------------------------------------------------------------------------
    // §进阶：用户构造抑制默认构造
    // -------------------------------------------------------------------------
    static_assert(!std::is_default_constructible_v<OnlyParamCtor>);
    OnlyParamCtor p{7};
    assert(p.v_ == 7);
    // OnlyParamCtor q; // ❌

    static_assert(std::is_default_constructible_v<RestoreDefault>);
    RestoreDefault r;
    assert(r.v_ == 0);
    RestoreDefault r2{9};
    assert(r2.v_ == 9);
    std::cout << "[advanced] restored default + param ctor ok\n";

    // -------------------------------------------------------------------------
    // §专家：值初始化 vs 默认初始化
    // -------------------------------------------------------------------------
    ImplicitDefault d{};  // 值初始化 → 零初始化成员
    assert(d.x == 0);
    // ImplicitDefault e;         // 默认初始化：e.x 不确定（勿读）

    // T() / T{} 对类类型调用默认构造；对聚合/标量是值初始化
    auto* heap = new WithNsdmi();  // 值初始化
    assert(heap->x == 42);
    delete heap;

    std::cout << "[expert] value-init ImplicitDefault.x=" << d.x << '\n';
    std::cout << "=== default_constructor: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/default_constructor", run>;

}  // namespace

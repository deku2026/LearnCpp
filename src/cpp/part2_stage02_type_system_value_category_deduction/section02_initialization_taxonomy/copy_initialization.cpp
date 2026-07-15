// Topic     : 拷贝初始化 copy initialization
// Doc       : 第2部分-阶段2 · 步骤 2.1
// cppreference: https://en.cppreference.com/cpp/language/copy_initialization
//
// 要点: T x = expr; / 传参按值 / 返回按值 / throw 等；
//       不使用 explicit 构造；可能发生用户定义转换；允许窄化。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <utility>

namespace {

struct ImplicitFromInt {
    int n;
    ImplicitFromInt(int v) : n{v} {}  // non-explicit
};

struct ExplicitFromInt {
    int n;
    explicit ExplicitFromInt(int v) : n{v} {}
};

struct Convertible {
    int n;
    operator ImplicitFromInt() const { return ImplicitFromInt{n}; }
};

void take_by_value(ImplicitFromInt x) {
    assert(x.n >= 0);
}

ImplicitFromInt make_one() {
    return 1;  // copy-init of return value from int
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [copy_initialization] ===\n";

    // -------------------------------------------------------------------------
    // 入门：等号语法
    // -------------------------------------------------------------------------
    [[maybe_unused]] int a = 42;
    [[maybe_unused]] double d = 3.14;
    std::string s = "text";
    [[maybe_unused]] int narrow = static_cast<int>(2.7);  // 允许窄化
    assert(a == 42 && s == "text" && narrow == 2);

    ImplicitFromInt i = 7;  // 调用 non-explicit 构造
    assert(i.n == 7);
    // ExplicitFromInt e = 7; // ❌ explicit 不能用于 copy-init
    ExplicitFromInt e{7};  // OK：direct-list-init
    assert(e.n == 7);
    std::cout << "[intro] T x = expr; explicit constructors excluded\n";

    // -------------------------------------------------------------------------
    // 进阶：传参、返回、转换序列
    // -------------------------------------------------------------------------
    take_by_value(3);  // 实参 copy-init 形参
    [[maybe_unused]] ImplicitFromInt r = make_one();
    assert(r.n == 1);

    Convertible c{10};
    [[maybe_unused]] ImplicitFromInt via_op = c;  // 用户定义转换
    assert(via_op.n == 10);

    // 拷贝初始化不一定调用拷贝构造：可直接从源构造目标（C++17 起 prvalue 更彻底）
    ImplicitFromInt j = ImplicitFromInt{5};
    assert(j.n == 5);
    std::cout << "[advanced] by-value param/return use copy-initialization\n";

    // -------------------------------------------------------------------------
    // 专家：copy-list-initialization T x = {args}
    // -------------------------------------------------------------------------
    // T x = {args} 是 list-init 的拷贝形式：仍禁窄化；且 explicit 构造通常不可用
    ImplicitFromInt k = {8};
    assert(k.n == 8);
    // ExplicitFromInt m = {8}; // ❌

    // 与直接初始化对比表：
    //   ImplicitFromInt x(1);   OK
    //   ImplicitFromInt x = 1;  OK
    //   ExplicitFromInt x(1);   OK
    //   ExplicitFromInt x = 1;  ill-formed

    std::cout << "[expert] copy-init is everywhere: =, pass-by-value, return\n";
    std::cout << "=== copy_initialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/copy_initialization", run>;

}  // namespace

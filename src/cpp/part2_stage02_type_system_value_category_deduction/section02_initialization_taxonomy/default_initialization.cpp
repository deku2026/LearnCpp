// Topic     : 默认初始化 default initialization
// Doc       : 第2部分-阶段2 · 步骤 2.1
// cppreference: https://en.cppreference.com/cpp/language/default_initialization
//
// 要点: T x; 对内置/数组元素 → 不初始化（不确定值）；类类型 → 默认构造；
//       静态/线程存储期对象还会先被零初始化（见 zero_initialization 专题）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

struct WithDefaultCtor {
    int n;
    WithDefaultCtor() : n{42} {}
};

struct TrivialNoCtor {
    int n;  // 无用户声明构造 → 默认初始化时成员也不初始化
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [default_initialization] ===\n";

    // -------------------------------------------------------------------------
    // 入门：T x; 对不同类型意味着什么
    // -------------------------------------------------------------------------
    // int garbage;          // 默认初始化：值不确定，读即 UB —— 不要读
    int safe = 0;       // 对照：声明即初始化
    std::string s;      // 类类型：调用默认构造 → 空串
    WithDefaultCtor w;  // 调用默认构造 → n==42
    assert(s.empty());
    assert(w.n == 42);
    assert(safe == 0);
    std::cout << "[intro] class types get default ctor; builtins do NOT zero\n";

    // -------------------------------------------------------------------------
    // 进阶：数组、new、成员
    // -------------------------------------------------------------------------
    // int arr[3];           // 每个元素默认初始化 → 不确定
    int arr_ok[3]{};  // 值初始化 → 全 0（对照）
    assert(arr_ok[0] == 0 && arr_ok[2] == 0);

    // new T 也是默认初始化
    auto* p = new int;    // *p 不确定 —— 不要读
    auto* q = new int{};  // 值初始化 → 0
    auto* r = new WithDefaultCtor;
    assert(*q == 0 && r->n == 42);
    delete p;
    delete q;
    delete r;

    // 类成员：若构造函数初始化列表没写，则按默认初始化成员
    // 对 int 成员 = 不确定（除非有默认成员初始化器 in-class）
    struct Holds {
        int a;                           // 无 in-class 初始化
        int b = 1;                       // 默认成员初始化器
        Holds() {}                       // a 默认初始化（不确定）；b 用 1
        explicit Holds(int x) : a{x} {}  // b 仍用默认成员初始化器
    };
    Holds h2{5};
    assert(h2.a == 5 && h2.b == 1);
    std::cout << "[advanced] new T vs new T{}; member default-init rules\n";

    // -------------------------------------------------------------------------
    // 专家：与零初始化、值初始化的边界
    // -------------------------------------------------------------------------
    // 静态存储期：int g; 先零初始化，再（可选）动态初始化 —— 读 g 安全且为 0
    // 自动存储期：int x; 仅默认初始化 —— 读 x 是 UB（C++23）
    // 值初始化 T x{}：内置清零；有默认构造的类调默认构造；无默认构造的类可能零初始化成员

    TrivialNoCtor t{};  // 值初始化：无默认构造时 → 零初始化成员
    assert(t.n == 0);
    // TrivialNoCtor u; // 默认初始化：u.n 不确定

    std::cout << "[expert] default-init ≠ zero-init for automatic builtins\n";
    std::cout << "=== default_initialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/default_initialization", run>;

}  // namespace

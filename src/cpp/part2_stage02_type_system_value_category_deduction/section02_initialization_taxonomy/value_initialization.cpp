// Topic     : 值初始化 value initialization
// Doc       : 第2部分-阶段2 · 步骤 2.1
// cppreference: https://en.cppreference.com/cpp/language/value_initialization
//
// 要点: T x{}; / T() / new T() —— 内置类型零初始化；
//       有用户提供默认构造的类 → 调默认构造；否则可能先零初始化再默认构造。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

struct EmptyUserDefault {
    int n;
    EmptyUserDefault() {}  // 用户提供：值初始化只调它，n 仍不确定！
};

struct InClassInit {
    int n = 99;
};

struct ImplicitDefault {
    int n;  // 无用户构造 → 值初始化时零初始化
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [value_initialization] ===\n";

    // -------------------------------------------------------------------------
    // 入门：内置与写法
    // -------------------------------------------------------------------------
    int a{};        // 0
    int b = int();  // 0（旧式值初始化）
    double d{};     // 0.0
    bool f{};       // false
    int* p{};       // nullptr
    assert(a == 0 && b == 0 && d == 0.0 && !f && p == nullptr);

    int arr[4]{};
    assert(arr[0] == 0 && arr[3] == 0);
    std::cout << "[intro] builtins and arrays zeroed by value-init\n";

    // -------------------------------------------------------------------------
    // 进阶：类类型规则
    // -------------------------------------------------------------------------
    std::string s{};  // 默认构造 → 空
    assert(s.empty());

    ImplicitDefault id{};
    assert(id.n == 0);  // 无用户默认构造 → 零初始化

    InClassInit ic{};
    assert(ic.n == 99);  // 默认构造使用 in-class 初始化器

    // 陷阱：用户提供了“空”默认构造后，值初始化不会先零初始化成员
    EmptyUserDefault eud{};
    // eud.n 仍然不确定——不要读。对比：
    EmptyUserDefault* heap = new EmptyUserDefault();  // 同样只调默认构造
    delete heap;
    std::cout << "[advanced] user-provided default ctor skips zeroing members\n";

    // new T() 是值初始化；new T 是默认初始化
    auto* zi = new int();
    assert(*zi == 0);
    delete zi;

    // -------------------------------------------------------------------------
    // 专家：T() 在表达式中是 prvalue；与直接初始化 T(args) 区分
    // -------------------------------------------------------------------------
    auto zero = int{};  // prvalue 物化/初始化
    assert(zero == 0);

    // 函数声明歧义：Widget w(); 是函数，不是值初始化——见 most_vexing_parse
    // 正确对象：Widget w{};

    // 聚合：空 {} 对每个元素值初始化
    struct Agg {
        int x;
        int y;
    };
    Agg g{};
    assert(g.x == 0 && g.y == 0);

    std::cout << "[expert] {} preferred over T() for value-init (and avoids MVP)\n";
    std::cout << "=== value_initialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/value_initialization", run>;

}  // namespace

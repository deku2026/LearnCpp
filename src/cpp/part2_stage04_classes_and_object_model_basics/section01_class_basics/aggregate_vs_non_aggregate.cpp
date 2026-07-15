// Topic     : 聚合 vs 非聚合
// Doc       : 第2部分-阶段4 · 步骤 1 / 与阶段2 聚合初始化对照
// cppreference: https://en.cppreference.com/cpp/language/aggregate_initialization
//
// 要点: 聚合可用 {a,b} 按成员初始化且无用户提供构造；用户声明构造/private 非静态数据
//       / virtual 等会破坏聚合性；C++20 起可有用户声明的 =default 构造仍是聚合（规则演进）。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>

namespace {

// 经典聚合：无用户构造、无 private/protected 非静态数据、无虚函数……
struct Agg {
    int x;
    int y;
    std::string name;
};

// 用户提供构造 → 通常不是聚合
struct NonAgg {
    int x;
    int y;
    NonAgg(int a, int b) : x(a), y(b) {}
};

// private 数据 → 非聚合
class Capsule {
    int secret_ = 0;

public:
    int get() const { return secret_; }
};

// 带 NSDMI 的聚合（C++14 起允许 NSDMI 的聚合）
struct AggWithNsdmi {
    int x = 1;
    int y = 2;
};

// 有基类的聚合（C++17 起允许 public 基类的聚合）
struct Base {
    int b = 0;
};
struct DerivedAgg : Base {
    int d = 0;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [aggregate_vs_non_aggregate] ===\n";

    // -------------------------------------------------------------------------
    // §入门：聚合初始化
    // -------------------------------------------------------------------------
    Agg a{10, 20, "ok"};
    assert(a.x == 10 && a.y == 20 && a.name == "ok");

    Agg a2{.x = 1, .y = 2, .name = "designated"};  // C++20 指定初始化（聚合）
    assert(a2.x == 1 && a2.name == "designated");
    std::cout << "[intro] Agg name=" << a.name << " designated=" << a2.name << '\n';

    // -------------------------------------------------------------------------
    // §进阶：非聚合不能「假装」聚合初始化路径
    // -------------------------------------------------------------------------
    NonAgg n{3, 4};  // 调构造函数，不是聚合初始化
    assert(n.x == 3 && n.y == 4);
    // NonAgg n2{3, 4, 5}; // ❌ 无匹配构造

    // Capsule c{1};       // ❌ 通常无法从外部聚合初始化 private
    [[maybe_unused]] Capsule c;
    assert(c.get() == 0);

    static_assert(std::is_aggregate_v<Agg>);
    static_assert(!std::is_aggregate_v<NonAgg>);
    static_assert(!std::is_aggregate_v<Capsule>);
    std::cout << "[advanced] is_aggregate Agg=1 NonAgg=0 Capsule=0\n";

    // -------------------------------------------------------------------------
    // §专家：NSDMI / 继承聚合 / 为何「写了构造」常失去 {} 便利
    // -------------------------------------------------------------------------
    [[maybe_unused]] AggWithNsdmi m{};  // 值初始化 → 用 NSDMI
    assert(m.x == 1 && m.y == 2);
    AggWithNsdmi m2{9};  // x=9, y 仍 NSDMI 2（聚合初始化规则）
    assert(m2.x == 9 && m2.y == 2);

    static_assert(std::is_aggregate_v<AggWithNsdmi>);
    static_assert(std::is_aggregate_v<DerivedAgg>);
    DerivedAgg d{{5}, 7};  // 基类子对象 + 成员（写法依编译器/标准细节，此处用嵌套）
    assert(d.b == 5 && d.d == 7);

    // 教学结论：需要不变量/封装 → 写构造 + private，放弃聚合；
    //           纯数据袋子 → 保持聚合 + 指定初始化，更清晰。
    std::cout << "[expert] NSDMI agg y=" << m2.y << " DerivedAgg d=" << d.d << '\n';
    std::cout << "=== aggregate_vs_non_aggregate: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/aggregate_vs_non_aggregate", run>;

}  // namespace

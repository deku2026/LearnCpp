// Topic    : 聚合判定与聚合初始化规则
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 9.2
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : aggregate_initialization_rules
// Topic id : part2/stage02/section09/aggregate_initialization_rules
// Refs     : https://en.cppreference.com/cpp/language/aggregate_initialization
//            ISO [dcl.init.aggr]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>

namespace {

// 典型聚合：无用户声明构造、无私有非静态数据、无虚函数…
struct Point {
    int x;
    int y;
    int z;
};

struct WithDefault {
    int a;
    int b = 5;  // 默认成员初始化器：仍可是聚合（C++14+）
};

// 非聚合：用户提供构造函数
struct NonAgg {
    int x;
    NonAgg(int v) : x(v) {}
};

// 非聚合：虚函数
struct HasVirtual {
    int x;
    virtual void f() {}
};

// 嵌套聚合
struct Segment {
    Point a;
    Point b;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [aggregate_initialization_rules] 入门：逐成员花括号 ===\n";
    {
        [[maybe_unused]] Point p1{1, 2, 3};  // x=1,y=2,z=3
        [[maybe_unused]] Point p2{1, 2, 0};  // z 值初始化 → 0
        (void)p2;
        [[maybe_unused]] Point p3{};           // 全 0
        [[maybe_unused]] int arr[3]{1, 2, 0};  // 数组也是聚合：1,2,0

        assert(p1.x == 1 && p1.y == 2 && p1.z == 3);
        assert(p2.x == 1 && p2.y == 2 && p2.z == 0);
        assert(p3.x == 0 && p3.y == 0 && p3.z == 0);
        assert(arr[0] == 1 && arr[1] == 2 && arr[2] == 0);

        static_assert(std::is_aggregate_v<Point>);
        static_assert(std::is_aggregate_v<int[3]>);
        std::cout << "[intro] missing members are value-initialized\n";
    }

    std::cout << "=== 进阶：默认成员初始化器；嵌套；非聚合对比 ===\n";
    {
        [[maybe_unused]] WithDefault w1{10};      // a=10, b 用默认 5
        [[maybe_unused]] WithDefault w2{10, 20};  // 覆盖 b
        [[maybe_unused]] WithDefault w3{};        // a=0, b=5
        assert(w1.a == 10 && w1.b == 5);
        assert(w2.a == 10 && w2.b == 20);
        assert(w3.a == 0 && w3.b == 5);
        static_assert(std::is_aggregate_v<WithDefault>);

        [[maybe_unused]] Segment s{{1, 2, 3}, {4, 5, 6}};
        assert(s.a.x == 1 && s.b.z == 6);

        // C++20 括号聚合初始化（了解）：Point p(1,2,3);
        Point p_paren(1, 2, 3);
        (void)p_paren;
        assert(p_paren.x == 1 && p_paren.z == 3);

        static_assert(!std::is_aggregate_v<NonAgg>);
        static_assert(!std::is_aggregate_v<HasVirtual>);
        NonAgg na{9};  // 走构造函数，不是聚合初始化
        assert(na.x == 9);

        std::cout << "[advanced] user ctor / virtual → not aggregate\n";
    }

    std::cout << "=== 专家：过多初始值非法；与拷贝初始化区分 ===\n";
    {
        // Point bad{1,2,3,4};  // ❌ 初始值多于成员
        Point p{1, 2, 3};
        [[maybe_unused]] Point copy = p;  // 拷贝初始化（不要求是聚合）
        assert(copy.y == 2);

        // 聚合初始化是步骤 2 初始化全家桶的一支
        // 空 {} → 每个成员值初始化；部分列表 → 剩余值初始化
        [[maybe_unused]] int zeros[4]{};
        assert(zeros[3] == 0);

        std::cout << "[expert] is_aggregate_v + brace init is the checklist\n";
    }

    std::cout << "[aggregate_initialization_rules] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section09/aggregate_initialization_rules", run>;

}  // namespace

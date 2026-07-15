// Topic     : 聚合初始化 aggregate initialization
// Doc       : 第2部分-阶段2 · 步骤 2.1 / 步骤 9 预告
// cppreference: https://en.cppreference.com/cpp/language/aggregate_initialization
//
// 要点: 聚合 = 无用户声明构造、无私有/保护非静态数据等（规则随标准演进）；
//       用 {} 按成员顺序初始化；不足的成员值初始化；C++20 有指定初始化器。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

struct Point {
    int x;
    int y;
};

struct Person {
    std::string name;
    int age{};
};

struct Nested {
    Point origin;
    int id;
};

// 有用户构造 → 不是聚合
struct NonAgg {
    int x;
    NonAgg(int v) : x{v} {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [aggregate_initialization] ===\n";

    // -------------------------------------------------------------------------
    // 入门：数组成员 / 简单 struct
    // -------------------------------------------------------------------------
    int a[3]{1, 2, 3};
    int b[3]{1};  // 其余值初始化 → 0
    assert(a[0] == 1 && a[2] == 3);
    assert(b[0] == 1 && b[1] == 0 && b[2] == 0);

    Point p{10, 20};
    Point q{5};  // y 值初始化 → 0
    assert(p.x == 10 && p.y == 20);
    assert(q.x == 5 && q.y == 0);

    static_assert(std::is_aggregate_v<Point>);
    static_assert(std::is_aggregate_v<Person>);
    static_assert(!std::is_aggregate_v<NonAgg>);
    std::cout << "[intro] array/struct brace init by member order\n";

    // -------------------------------------------------------------------------
    // 进阶：嵌套、字符串成员、空 {}
    // -------------------------------------------------------------------------
    Nested n{{1, 2}, 99};
    assert(n.origin.x == 1 && n.origin.y == 2 && n.id == 99);

    Person alice{"Alice", 30};
    Person bob{"Bob"};  // age 用 in-class / 值初始化 → 0
    assert(alice.name == "Alice" && alice.age == 30);
    assert(bob.name == "Bob" && bob.age == 0);

    Point zero{};
    assert(zero.x == 0 && zero.y == 0);
    std::cout << "[advanced] nested aggregates; trailing members value-initialized\n";

    // -------------------------------------------------------------------------
    // 专家：C++20 指定初始化器（designated initializers）
    // -------------------------------------------------------------------------
    Point d{.x = 3, .y = 4};
    assert(d.x == 3 && d.y == 4);
    // 指定顺序必须与声明顺序一致；不能混用乱序（C++ 比 C 更严）
    Point d2{.x = 7};  // y → 0
    assert(d2.x == 7 && d2.y == 0);

    // 数组的“聚合”与 string 字面值
    char buf[]{"hi"};  // 含 '\0'
    assert(buf[0] == 'h' && buf[2] == '\0');

    // 对比：NonAgg 不能聚合初始化
    NonAgg na{42};  // 这是调用构造函数的列表初始化，不是聚合初始化
    assert(na.x == 42);

    std::cout << "[expert] C++20 designated init; is_aggregate_v to check\n";
    std::cout << "=== aggregate_initialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/aggregate_initialization", run>;

}  // namespace

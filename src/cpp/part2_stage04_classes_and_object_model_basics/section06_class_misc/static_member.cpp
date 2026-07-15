// Topic     : static 成员
// Doc       : 第2部分-阶段4 · 步骤 8.1
// cppreference: https://en.cppreference.com/cpp/language/static
//
// 要点: static 数据共享一份；static 函数无 this；C++17 inline static 类内定义。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

class Counter {
    static inline int count_ = 0;  // C++17
    int id_;

public:
    Counter() : id_(++count_) {}
    ~Counter() { --count_; }

    static int total() { return count_; }
    int id() const { return id_; }

    // static 只能直接碰 static 成员（无 this）
    static std::string banner() { return "Counter"; }
};

// 非 inline 的旧写法示意（若不用 inline 需类外定义）：
// int Counter::count_ = 0;

struct Factory {
    static Counter make() { return Counter{}; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [static_member] ===\n";

    // -------------------------------------------------------------------------
    // §入门：共享计数
    // -------------------------------------------------------------------------
    assert(Counter::total() == 0);
    Counter a, b;
    assert(Counter::total() == 2);
    assert(a.id() != b.id());
    std::cout << "[intro] total=" << Counter::total() << " ids=" << a.id() << ',' << b.id() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：经对象调用 static（语法允许，语义仍无 this）
    // -------------------------------------------------------------------------
    assert(a.total() == Counter::total());
    assert(Counter::banner() == "Factory" || Counter::banner() == "Counter");
    assert(Counter::banner() == "Counter");
    {
        Counter c;
        assert(Counter::total() == 3);
    }
    assert(Counter::total() == 2);
    std::cout << "[advanced] after nested dtor total=" << Counter::total() << '\n';

    // -------------------------------------------------------------------------
    // §专家：工厂 + 生命周期
    // -------------------------------------------------------------------------
    auto d = Factory::make();
    assert(Counter::total() == 3);
    (void)d;

    std::cout << "[expert] Factory::make total=" << Counter::total() << '\n';
    std::cout << "=== static_member: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/static_member", run>;

}  // namespace

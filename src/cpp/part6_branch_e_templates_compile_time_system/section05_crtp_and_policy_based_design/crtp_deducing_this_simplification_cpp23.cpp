// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E5 CRTP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section05_crtp_and_policy_based_design
// Item     : crtp_deducing_this_simplification_cpp23
// Topic id : part6/e/section05/crtp_deducing_this_simplification_cpp23
//
// 要点: C++23 deducing this 可去掉 CRTP 基类模板参数,
//       用 this Self& 直接拿到最派生类型。
// 参考: P0847R7; cppreference explicit object parameter

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

// --- 经典 CRTP ---
template <typename D>
struct ClassicAdd {
    D operator+(const D& other) const {
        D tmp = static_cast<const D&>(*this);
        tmp.add_assign(other);
        return tmp;
    }
};

struct ClassicVec : ClassicAdd<ClassicVec> {
    int x = 0;
    int y = 0;
    ClassicVec() = default;
    ClassicVec(int x_, int y_) : x(x_), y(y_) {}
    void add_assign(const ClassicVec& o) {
        x += o.x;
        y += o.y;
    }
};

// --- C++23 deducing this: 无需模板基类 ---
struct ModernVec {
    int x = 0, y = 0;

    void add_assign(const ModernVec& o) {
        x += o.x;
        y += o.y;
    }

    // Self 推导为 ModernVec (或更派生类型, 若被继承)
    template <typename Self>
    auto operator+(this const Self& self, const Self& other) {
        Self tmp = self;
        tmp.add_assign(other);
        return tmp;
    }

    // 同时折叠 const / 引用限定
    template <typename Self>
    auto&& get_x(this Self&& self) {
        return std::forward<Self>(self).x;
    }
};

// mixin 风格: 自由函数式 interface 注入
struct Named {
    std::string name;

    template <typename Self>
    std::string label(this const Self& self) {
        // 若派生类有 extra_tag() 可扩展; 这里简单返回 name
        return self.name;
    }
};

struct Player : Named {
    int score = 0;
};

// 递归 lambda 也靠 deducing this
int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E5 CRTP via deducing this (C++23) ===\n";

    ClassicVec a(1, 2);
    ClassicVec b(3, 4);
    [[maybe_unused]] auto c = a + b;
    assert(c.x == 4 && c.y == 6);

    ModernVec m1{1, 2}, m2{10, 20};
    [[maybe_unused]] auto m3 = m1 + m2;
    assert(m3.x == 11 && m3.y == 22);

    ModernVec m{5, 0};
    assert(m.get_x() == 5);
    m.get_x() = 9;
    assert(m.x == 9);

    Player p;
    p.name = "Ada";
    assert(p.label() == "Ada");

    // 递归阶乘 lambda
    [[maybe_unused]] auto fact = [](this auto self, int n) -> int { return n <= 1 ? 1 : n * self(n - 1); };
    assert(fact(5) == 120);

    std::cout << "  classic CRTP needs template base; deducing this does not\n";
    std::cout << "  also collapses ref-qualified getters into one template\n";
    std::cout << "crtp_deducing_this_simplification_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section05/crtp_deducing_this_simplification_cpp23", run>;

}  // namespace

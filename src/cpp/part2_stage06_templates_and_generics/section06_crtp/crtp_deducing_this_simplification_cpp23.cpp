// Topic    : C++23 deducing this 简化 CRTP —— 基类不必再是模板
// Doc      : 第2部分-阶段6 · 步骤 12.3
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_deducing_this_simplification_cpp23
// Topic id : part2/stage06/section06/crtp_deducing_this_simplification_cpp23
// Refs     : https://en.cppreference.com/w/cpp/language/member_functions
//            https://wg21.link/P0847R7
//            ISO [dcl.fct]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 传统 CRTP vs deducing this 对照骨架
// ---------------------------------------------------------------------------

// 传统：
// template<class D> struct Shape { void draw(){ static_cast<D*>(this)->impl(); } };
// struct Circle : Shape<Circle> { void impl(); };

// C++23：
struct Shape23 {
    void draw(this const auto& self) { self.draw_impl(); }
    auto name(this const auto& self) { return self.name_impl(); }
};

struct Circle23 : Shape23 {
    void draw_impl() const { std::cout << "  Circle23\n"; }
    std::string name_impl() const { return "circle23"; }
};

struct Square23 : Shape23 {
    void draw_impl() const { std::cout << "  Square23\n"; }
    std::string name_impl() const { return "square23"; }
};

// ---------------------------------------------------------------------------
// §进阶 — mixin 也变普通基类
// ---------------------------------------------------------------------------

struct Equality23 {
    bool operator==(this const auto& self, const auto& other) { return self.value == other.value; }
    bool operator!=(this const auto& self, const auto& other) { return !(self == other); }
};

struct Id : Equality23 {
    int value = 0;
    explicit Id(int v) : value(v) {}
};

struct Fluent23 {
    auto&& set(this auto&& self, int v) {
        self.x = v;
        return std::forward<decltype(self)>(self);
    }
};

struct Point : Fluent23 {
    int x = 0;
};

// ---------------------------------------------------------------------------
// §专家 — 简化对照表（运行期打印要点）
// ---------------------------------------------------------------------------

template <typename D>
struct OldCRTP {
    int id() const { return static_cast<const D*>(this)->id_impl(); }
};
struct OldNode : OldCRTP<OldNode> {
    int id_impl() const { return 7; }
};

struct NewBase {
    int id(this const auto& self) { return self.id_impl(); }
};
struct NewNode : NewBase {
    int id_impl() const { return 7; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [crtp_deducing_this] 入门：普通继承 ===\n";
    Circle23 c;
    Square23 s;
    c.draw();
    s.draw();
    assert(c.name() == "circle23");
    assert(s.name() == "square23");
    // 继承写法：struct D : Shape23  —— 不用 Shape23<D>
    std::cout << "no Base<Derived> noise\n";

    std::cout << "=== 进阶：Equality / Fluent ===\n";
    Id a{1}, b{2}, d{1};
    assert(a == d);
    assert(a != b);
    Point p;
    p.set(42).set(43);
    assert(p.x == 43);
    std::cout << "mixin without templates OK\n";

    std::cout << "=== 专家：对照传统 CRTP ===\n";
    [[maybe_unused]] OldNode on;
    [[maybe_unused]] NewNode nn;
    assert(on.id() == 7 && nn.id() == 7);
    std::cout << "传统: Base 必须是模板, static_cast, 派生写 Base<D>\n";
    std::cout << "C++23: Base 普通类, self 直接是派生类型, 普通继承\n";
    // 编译器: MSVC 19.32+ / GCC 14+ / Clang 18+ , -std=c++23
    std::cout << "=== crtp_deducing_this_simplification_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_deducing_this_simplification_cpp23", run>;

}  // namespace

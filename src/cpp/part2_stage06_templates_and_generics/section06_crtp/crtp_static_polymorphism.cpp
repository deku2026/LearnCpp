// Topic    : CRTP 静态多态 —— Base<Derived> + static_cast 绑定
// Doc      : 第2部分-阶段6 · 步骤 12.1–12.2
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_static_polymorphism
// Topic id : part2/stage06/section06/crtp_static_polymorphism
// Refs     : https://en.cppreference.com/w/cpp/language/crtp
//            《C++ Templates: The Complete Guide》CRTP
//            ISO [class.derived]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — Shape<Derived> 静态分发 draw
// ---------------------------------------------------------------------------

template <typename Derived>
struct Shape {
    void draw() const {
        // 编译期绑定到派生类实现，无虚表
        static_cast<const Derived*>(this)->draw_impl();
    }

    std::string name() const { return static_cast<const Derived*>(this)->name_impl(); }
};

struct Circle : Shape<Circle> {
    void draw_impl() const { std::cout << "  draw Circle\n"; }
    std::string name_impl() const { return "circle"; }
};

struct Square : Shape<Square> {
    void draw_impl() const { std::cout << "  draw Square\n"; }
    std::string name_impl() const { return "square"; }
};

// ---------------------------------------------------------------------------
// §进阶 — 泛型算法只依赖接口约定
// ---------------------------------------------------------------------------

template <typename D>
void render(const Shape<D>& s) {
    s.draw();
}

template <typename D>
int area_tag(const Shape<D>& s) {
    // 不同派生可提供 area_impl；这里用 name 演示
    return static_cast<int>(s.name().size());
}

// ---------------------------------------------------------------------------
// §专家 — 每个派生类是独立层次；可内联；错误派生名是静默灾难
// ---------------------------------------------------------------------------

template <typename D>
struct Counted : Shape<D> {
    static inline int draws = 0;
    void touch() {
        ++draws;
        static_cast<D*>(this)->draw_impl();
    }
};

struct Dot : Counted<Dot> {
    void draw_impl() const { std::cout << "  dot\n"; }
};

// ⚠️ 若写成 struct Bad : Shape<Circle> 会转型到错误类型 → UB
// 现代可用 static_assert(std::is_base_of_v<Shape<D>, D>) 之类防御

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [crtp_static_polymorphism] 入门 ===\n";
    Circle c;
    Square s;
    c.draw();
    s.draw();
    assert(c.name() == "circle");
    assert(s.name() == "square");

    std::cout << "=== 进阶：泛型 render ===\n";
    render(c);
    render(s);
    assert(area_tag(c) == static_cast<int>(std::string{"circle"}.size()));

    // Circle 与 Square 没有共同的非模板基类指针类型
    // std::vector<Shape*> 不可行（Shape 是模板）
    std::vector<std::string> names{c.name(), s.name()};
    assert(names.size() == 2);
    std::cout << "names collected by value OK\n";

    std::cout << "=== 专家：可内联静态分发 ===\n";
    Dot d;
    d.touch();
    d.touch();
    assert(Counted<Dot>::draws == 2);
    // 对比虚函数：这里 draw_impl 地址在编译期已知，优化器可内联
    std::cout << "=== crtp_static_polymorphism: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_static_polymorphism", run>;

}  // namespace

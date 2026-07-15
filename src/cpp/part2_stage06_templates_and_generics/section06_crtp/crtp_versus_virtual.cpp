// Topic    : CRTP 静态多态 vs 虚函数动态多态 —— 取舍
// Doc      : 第2部分-阶段6 · 步骤 12.4
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_versus_virtual
// Topic id : part2/stage06/section06/crtp_versus_virtual
// Refs     : https://en.cppreference.com/w/cpp/language/virtual
//            《C++ Software Design》(Iglberger)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 两套平行实现同一「画图」接口
// ---------------------------------------------------------------------------

// 动态多态
struct DynShape {
    virtual ~DynShape() = default;
    virtual void draw() const = 0;
    virtual std::string name() const = 0;
};

struct DynCircle : DynShape {
    void draw() const override { std::cout << "  dyn circle\n"; }
    std::string name() const override { return "dyn-circle"; }
};

struct DynSquare : DynShape {
    void draw() const override { std::cout << "  dyn square\n"; }
    std::string name() const override { return "dyn-square"; }
};

// 静态多态 CRTP
template <typename D>
struct StaticShape {
    void draw() const { static_cast<const D*>(this)->draw_impl(); }
    std::string name() const { return static_cast<const D*>(this)->name_impl(); }
};

struct StaticCircle : StaticShape<StaticCircle> {
    void draw_impl() const { std::cout << "  static circle\n"; }
    std::string name_impl() const { return "static-circle"; }
};

struct StaticSquare : StaticShape<StaticSquare> {
    void draw_impl() const { std::cout << "  static square\n"; }
    std::string name_impl() const { return "static-square"; }
};

// ---------------------------------------------------------------------------
// §进阶 — 异质容器：只有动态多态轻松做到
// ---------------------------------------------------------------------------

void paint_all(const std::vector<std::unique_ptr<DynShape>>& shapes) {
    for (const auto& s : shapes) {
        s->draw();
    }
}

template <typename D>
void paint_one(const StaticShape<D>& s) {
    s.draw();
}

// ---------------------------------------------------------------------------
// §专家 — 选择清单（用输出固化经验法则）
// ---------------------------------------------------------------------------

struct Decision {
    bool type_known_at_compile_time = false;
    bool need_heterogeneous_container = false;
    bool need_plugin_runtime_extension = false;
    bool need_zero_overhead_inline = false;

    std::string recommend() const {
        if (need_heterogeneous_container || need_plugin_runtime_extension) {
            return "virtual";
        }
        if (type_known_at_compile_time && need_zero_overhead_inline) {
            return "crtp";
        }
        return "either";
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [crtp_versus_virtual] 入门：两套实现 ===\n";
    DynCircle dc;
    StaticCircle sc;
    dc.draw();
    sc.draw();
    assert(dc.name() == "dyn-circle");
    assert(sc.name() == "static-circle");

    std::cout << "=== 进阶：异质容器只有虚函数自然 ===\n";
    std::vector<std::unique_ptr<DynShape>> gallery;
    gallery.push_back(std::make_unique<DynCircle>());
    gallery.push_back(std::make_unique<DynSquare>());
    paint_all(gallery);
    assert(gallery.size() == 2);

    // CRTP：每个 StaticShape<D> 是不同基类，不能 vector<StaticShape*>
    StaticSquare ss;
    paint_one(sc);
    paint_one(ss);
    std::cout << "dyn gallery size=" << gallery.size() << '\n';

    std::cout << "=== 专家：取舍表 ===\n";
    std::cout << "  绑定时机: CRTP=编译期  虚函数=运行期\n";
    std::cout << "  开销:     CRTP=零(可内联) 虚函数=虚表+难内联\n";
    std::cout << "  代码体积: CRTP 每类型一份  虚函数共享\n";
    std::cout << "  异质容器: CRTP 难  虚函数 易\n";

    Decision d1{.type_known_at_compile_time = true, .need_zero_overhead_inline = true};
    assert(d1.recommend() == "crtp");
    Decision d2{.need_heterogeneous_container = true};
    assert(d2.recommend() == "virtual");
    Decision d3{.need_plugin_runtime_extension = true};
    assert(d3.recommend() == "virtual");
    std::cout << "recommend mixin=" << d1.recommend() << " gallery=" << d2.recommend() << '\n';
    std::cout << "=== crtp_versus_virtual: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_versus_virtual", run>;

}  // namespace

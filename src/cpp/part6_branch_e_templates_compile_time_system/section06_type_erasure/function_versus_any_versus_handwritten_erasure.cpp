// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E6 type erasure)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : function_versus_any_versus_handwritten_erasure
// Topic id : part6/e/section06/function_versus_any_versus_handwritten_erasure
//
// 要点: std::function = 可调用擦除; std::any = 任意值擦除;
//       手写擦除 = vtable + 拥有权, 可定制接口。
// 参考: Sean Parent "Inheritance Is The Base Class of Evil"; Iglberger

#include "learn/topic_registry.hpp"

#include <any>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

// 手写"可绘制"类型擦除
class Drawable {
    struct Concept {
        virtual ~Concept() = default;
        virtual void draw() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    template <typename T>
    struct Model : Concept {
        T data;
        explicit Model(T v) : data(std::move(v)) {}
        void draw() const override { data.draw(); }
        std::unique_ptr<Concept> clone() const override { return std::make_unique<Model>(*this); }
    };

    std::unique_ptr<Concept> self_;

public:
    template <typename T>
    Drawable(T v) : self_(std::make_unique<Model<T>>(std::move(v))) {}

    Drawable(const Drawable& o) : self_(o.self_->clone()) {}
    Drawable& operator=(Drawable o) {
        self_ = std::move(o.self_);
        return *this;
    }
    Drawable(Drawable&&) noexcept = default;
    Drawable& operator=(Drawable&&) noexcept = default;

    void draw() const { self_->draw(); }
};

struct Circle {
    void draw() const { /* side-effect free for test */ }
    int r = 1;
};
struct Text {
    std::string s;
    void draw() const {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E6 function vs any vs handwritten erasure ===\n";

    // std::function: 统一调用签名
    std::function<int(int)> f = [](int x) { return x + 1; };
    f = [n = 10](int x) { return x + n; };
    assert(f(5) == 15);

    // std::any: 类型擦除的值容器
    std::any a = 42;
    assert(std::any_cast<int>(a) == 42);
    a = std::string{"hi"};
    assert(std::any_cast<std::string>(a) == "hi");
    try {
        (void)std::any_cast<int>(a);
        assert(false);
    } catch (const std::bad_any_cast&) {
        std::cout << "  any_cast type mismatch OK\n";
    }

    // 手写: 异构容器 + 值语义
    std::vector<Drawable> scene;
    scene.emplace_back(Circle{2});
    scene.emplace_back(Text{"hello"});
    for (const auto& d : scene) {
        d.draw();
    }
    auto copy = scene;
    assert(copy.size() == 2);

    std::cout << "  function: callable only; any: no interface;"
                 " handwritten: custom ops + value semantics\n";
    std::cout << "function_versus_any_versus_handwritten_erasure: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/e/section06/function_versus_any_versus_handwritten_erasure", run>;

}  // namespace

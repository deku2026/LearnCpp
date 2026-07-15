// Topic     : 虚函数
// Doc       : 第2部分-阶段4 · 步骤 9.1
// cppreference: https://en.cppreference.com/cpp/language/virtual
//
// 要点: virtual 启用运行期分发；经基类指针/引用调到最终覆盖函数；构造/析构中虚调用不派发到派生。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const { return 0.0; }
    virtual std::string name() const { return "Shape"; }
};

class Circle : public Shape {
    double r_;

public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
    std::string name() const override { return "Circle"; }
};

class Rect : public Shape {
    double w_, h_;

public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    std::string name() const override { return "Rect"; }
};

struct Base {
    Base() { /* init(); 勿依赖派生 */ }
    virtual void init() {}
    virtual std::string id() const { return "Base"; }
    virtual ~Base() = default;
};

struct Derived : Base {
    std::string id() const override { return "Derived"; }
    void init() override {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [virtual_functions] ===\n";

    // -------------------------------------------------------------------------
    // §入门：基类引用分发
    // -------------------------------------------------------------------------
    Circle c{2.0};
    Shape& s = c;
    assert(s.name() == "Circle");
    assert(s.area() > 12.0 && s.area() < 13.0);
    std::cout << "[intro] " << s.name() << " area~" << s.area() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：容器里的多态
    // -------------------------------------------------------------------------
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rect>(3.0, 4.0));
    double sum = 0.0;
    for (const auto& sh : shapes) {
        sum += sh->area();
    }
    assert(sum > 12.0 && sum < 16.0);
    std::cout << "[advanced] areas sum~" << sum << '\n';

    // -------------------------------------------------------------------------
    // §专家：非虚对比 + 构造期
    // -------------------------------------------------------------------------
    Derived d;
    Base& br = d;
    assert(br.id() == "Derived");

    // 无 virtual 的静态绑定示意：
    struct NVBase {
        std::string id() const { return "NVBase"; }
    };
    struct NVDer : NVBase {
        std::string id() const { return "NVDer"; }
    };
    NVDer nd;
    NVBase& nbr = nd;
    assert(nbr.id() == "NVBase");  // 非虚：按静态类型

    std::cout << "[expert] virtual vs static binding demonstrated\n";
    std::cout << "=== virtual_functions: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/virtual_functions", run>;

}  // namespace

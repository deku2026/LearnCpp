// Topic     : 虚函数
// Doc       : 第2部分-阶段4 · 步骤 9.1 / 9.4
// cppreference: https://en.cppreference.com/cpp/language/virtual
//
// 要点: virtual 启用运行期分发；经基类指针/引用调到最终覆盖函数；
//       ⚠️ 构造/析构中调用虚函数不会派发到派生（派生子对象尚未建成/已销毁）。

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

// 构造期虚调用：记录「实际调用的是哪一层」
struct Base {
    std::string* log = nullptr;

    explicit Base(std::string* l) : log(l) {
        // ⚠️ 构造里调虚函数 → 只会落到「当前正在构造的类」版本
        boot();
    }

    virtual void boot() {
        if (log) {
            *log += "Base::boot;";
        }
    }

    virtual std::string id() const { return "Base"; }
    virtual ~Base() {
        // 析构里同样不会派发到派生
        shutdown();
    }

    virtual void shutdown() {
        if (log) {
            *log += "Base::shutdown;";
        }
    }
};

struct Derived : Base {
    explicit Derived(std::string* l) : Base(l) {}

    void boot() override {
        if (log) {
            *log += "Derived::boot;";
        }
    }

    void shutdown() override {
        if (log) {
            *log += "Derived::shutdown;";
        }
    }

    std::string id() const override { return "Derived"; }
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

    Derived d_live{nullptr};
    Base& br = d_live;
    assert(br.id() == "Derived");  // 构造完成后：正常动态分发
    std::cout << "[advanced] areas sum~" << sum << " id=" << br.id() << '\n';

    // -------------------------------------------------------------------------
    // §专家：非虚静态绑定 + 构造/析构中的虚调用陷阱（文档 9.4 验收）
    // -------------------------------------------------------------------------
    struct NVBase {
        std::string id() const { return "NVBase"; }
    };
    struct NVDer : NVBase {
        std::string id() const { return "NVDer"; }
    };
    NVDer nd;
    NVBase& nbr = nd;
    assert(nbr.id() == "NVBase");  // 非虚：按静态类型

    std::string log;
    {
        Derived tmp{&log};
        // 构造 Base 时 Derived 尚未建成 → 只应看到 Base::boot
        assert(log.find("Base::boot;") != std::string::npos);
        assert(log.find("Derived::boot;") == std::string::npos);
        assert(tmp.id() == "Derived");  // 构造完成后可正常派发
    }
    // 析构顺序：先 ~Derived（可调 Derived::shutdown），再 ~Base（此时派生已毁，
    // Base::shutdown 再虚调只会落到 Base）。本演示在 ~Base 里调 shutdown。
    assert(log.find("Base::shutdown;") != std::string::npos);

    // 在抽象类构造/析构里对纯虚做虚调用是 UB——永远不要这么做。
    std::cout << "[expert] ctor virtual log=" << log << " (no Derived::boot during Base ctor)\n";
    std::cout << "=== virtual_functions: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/virtual_functions", run>;

}  // namespace

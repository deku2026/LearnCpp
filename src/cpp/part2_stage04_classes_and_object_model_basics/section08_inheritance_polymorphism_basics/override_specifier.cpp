// Topic     : override 说明符
// Doc       : 第2部分-阶段4 · 步骤 9.2
// cppreference: https://en.cppreference.com/cpp/language/override
//
// 要点: 标明意图重写；签名不符则编译失败；重写虚函数一律加 override。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

class Base {
public:
    virtual ~Base() = default;
    virtual std::string f() const { return "Base::f"; }
    virtual void g(int) {}
    virtual int h() { return 0; }
};

class Derived : public Base {
public:
    std::string f() const override { return "Derived::f"; }
    void g(int) override {}
    int h() override { return 1; }

    // 若误写签名，override 会抓出来：
    // void g() const override;     // ❌ 基类无匹配
    // std::string f() override;    // ❌ 缺 const
};

class Further : public Derived {
public:
    std::string f() const override { return "Further::f"; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [override_specifier] ===\n";

    // -------------------------------------------------------------------------
    // §入门：正确重写
    // -------------------------------------------------------------------------
    Derived d;
    Base& b = d;
    assert(b.f() == "Derived::f");
    assert(d.h() == 1);
    std::cout << "[intro] " << b.f() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：多层覆盖
    // -------------------------------------------------------------------------
    Further fur;
    Base& b2 = fur;
    assert(b2.f() == "Further::f");
    std::cout << "[advanced] " << b2.f() << '\n';

    // -------------------------------------------------------------------------
    // §专家：为什么必须写 override
    // -------------------------------------------------------------------------
    // 历史坑：以为重写了，其实因 const/参数类型 silently 定义了新函数，基类版本仍被调用。
    // override 把该类 bug 变成编译错误。Qt 的 paintEvent 等同样该写 override。
    struct Almost : Base {
        // std::string f() { return "oops"; } // 无 override：实际隐藏/新函数，不覆盖 const 版本
        std::string f() const override { return "ok"; }
    };
    Almost a;
    assert(static_cast<Base&>(a).f() == "ok");

    std::cout << "[expert] override prevents silent non-overrides\n";
    std::cout << "=== override_specifier: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/override_specifier", run>;

}  // namespace

// Topic     : final 说明符
// Doc       : 第2部分-阶段4 · 步骤 9.2
// cppreference: https://en.cppreference.com/cpp/language/final
//
// 要点: final 在虚函数上禁止再覆盖；在类上禁止再继承。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

class Base {
public:
    virtual ~Base() = default;
    virtual std::string f() const { return "Base"; }
    virtual std::string g() const { return "Base::g"; }
};

class Derived : public Base {
public:
    std::string f() const final { return "Derived::f"; }  // 禁止再覆盖 f
    std::string g() const override { return "Derived::g"; }
};

class Further : public Derived {
public:
    // std::string f() const override { return "nope"; } // ❌ f 已 final
    std::string g() const override { return "Further::g"; }
};

class Sealed final : public Base {
public:
    std::string f() const override { return "Sealed"; }
};

// class More : public Sealed {}; // ❌ Sealed 是 final 类

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [final_specifier] ===\n";

    // -------------------------------------------------------------------------
    // §入门：函数 final
    // -------------------------------------------------------------------------
    Further fur;
    Base& b = fur;
    assert(b.f() == "Derived::f");  // f 停在 Derived
    assert(b.g() == "Further::g");
    std::cout << "[intro] f=" << b.f() << " g=" << b.g() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：类 final
    // -------------------------------------------------------------------------
    Sealed s;
    assert(s.f() == "Sealed");
    Base& bs = s;
    assert(bs.f() == "Sealed");
    std::cout << "[advanced] sealed class f=" << bs.f() << '\n';

    // -------------------------------------------------------------------------
    // §专家：用途
    // -------------------------------------------------------------------------
    // 1) 设计上关闭扩展点（安全/API 稳定）
    // 2) 帮助编译器去虚化优化（有时）
    // 3) 与 override 组合：void f() override final;
    std::cout << "[expert] final closes extension points\n";
    std::cout << "=== final_specifier: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/final_specifier", run>;

}  // namespace

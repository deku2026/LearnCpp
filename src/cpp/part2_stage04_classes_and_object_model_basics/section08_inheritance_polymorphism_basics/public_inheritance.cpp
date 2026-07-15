// Topic     : public 继承
// Doc       : 第2部分-阶段4 · 步骤 9.1
// cppreference: https://en.cppreference.com/cpp/language/derived_class
//
// 要点: is-a 关系；public 继承保持基类接口访问级别；基类引用/指针绑定派生对象。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

class Animal {
    std::string name_;

public:
    explicit Animal(std::string name) : name_(std::move(name)) {}
    virtual ~Animal() = default;

    const std::string& name() const { return name_; }
    virtual std::string speak() const { return "..."; }
};

class Dog : public Animal {
public:
    explicit Dog(std::string name) : Animal(std::move(name)) {}
    std::string speak() const override { return "woof"; }
    void wag() const {}  // 派生特有
};

class Cat : public Animal {
public:
    explicit Cat(std::string name) : Animal(std::move(name)) {}
    std::string speak() const override { return "meow"; }
};

std::string call_speak(const Animal& a) {
    return a.speak();
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [public_inheritance] ===\n";

    // -------------------------------------------------------------------------
    // §入门：派生对象即基类
    // -------------------------------------------------------------------------
    Dog d{"Rex"};
    assert(d.name() == "Rex");
    assert(d.speak() == "woof");
    d.wag();
    std::cout << "[intro] " << d.name() << " says " << d.speak() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：基类引用绑定
    // -------------------------------------------------------------------------
    Animal& ar = d;
    assert(ar.speak() == "woof");  // 虚分发
    assert(call_speak(d) == "woof");
    assert(call_speak(Cat{"Mimi"}) == "meow");
    std::cout << "[advanced] polymorphic call_speak ok\n";

    // -------------------------------------------------------------------------
    // §专家：切片警告（按值传基类）
    // -------------------------------------------------------------------------
    Animal sliced = d;                // 对象切片：只剩 Animal 部分
    assert(sliced.speak() == "...");  // 不是 woof
    // 传参用 const Animal& / 智能指针，避免切片
    std::cout << "[expert] sliced.speak=" << sliced.speak() << " (lost Dog)\n";
    std::cout << "=== public_inheritance: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/public_inheritance", run>;

}  // namespace

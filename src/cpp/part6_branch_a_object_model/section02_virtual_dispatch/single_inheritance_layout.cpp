// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A4 单继承布局)
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : single_inheritance_layout
// Topic id : part6/a/section02/single_inheritance_layout
//
// 要点: 单继承下派生对象 = 基类子对象(含共享 vptr) + 派生成员;
//       基类指针/引用与完整对象地址通常相同 (主基类在偏移 0);
//       覆盖不增加 vptr 个数; 析构链顺序: 派生 → 基类。
// 参考: [class.derived] [class.cdtor]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Animal {
    int legs = 4;
    virtual ~Animal() = default;
    virtual std::string name() const { return "Animal"; }
    virtual int speed() const { return 1; }
};

struct Mammal : Animal {
    bool warm_blooded = true;
    std::string name() const override { return "Mammal"; }
    int speed() const override { return 5; }
};

struct Dog : Mammal {
    std::string breed = "Husky";
    std::string name() const override { return "Dog"; }
    int speed() const override { return 20; }
};

// 构造/析构期间的虚分派: 动态类型是“正在构造/析构的那一层”
struct TraceBase {
    const char* tag = "base";
    TraceBase() { tag = who(); }
    virtual ~TraceBase() { /* 析构时 who() 仍见 Base 版 */ }
    virtual const char* who() const { return "TraceBase"; }
};

struct TraceDerived : TraceBase {
    TraceDerived() { tag = who(); }
    const char* who() const override { return "TraceDerived"; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== single inheritance layout ===\n";

    Dog dog;
    Mammal* as_mammal = &dog;
    Animal* as_animal = &dog;

    // --- 地址: 单继承主路径上, 转基类通常不改指针值 ---
    const auto dog_addr = reinterpret_cast<std::uintptr_t>(&dog);
    const auto mam_addr = reinterpret_cast<std::uintptr_t>(as_mammal);
    const auto ani_addr = reinterpret_cast<std::uintptr_t>(as_animal);
    std::cout << "  &Dog     = 0x" << std::hex << dog_addr << std::dec << '\n';
    std::cout << "  &Mammal  = 0x" << std::hex << mam_addr << std::dec << '\n';
    std::cout << "  &Animal  = 0x" << std::hex << ani_addr << std::dec << '\n';
    assert(dog_addr == mam_addr);
    assert(dog_addr == ani_addr);

    // 虚分派沿单链覆盖
    assert(as_animal->name() == "Dog");
    assert(as_animal->speed() == 20);
    assert(as_mammal->name() == "Dog");

    // --- sizeof 单调不减: 派生至少容纳基类子对象 + 新成员 ---
    std::cout << "  sizeof(Animal)=" << sizeof(Animal) << " Mammal=" << sizeof(Mammal) << " Dog=" << sizeof(Dog)
              << '\n';
    assert(sizeof(Mammal) >= sizeof(Animal));
    assert(sizeof(Dog) >= sizeof(Mammal));

    // 基类子对象可绑定引用, 仍共享同一完整对象
    Animal& ar = dog;
    assert(&ar == as_animal);
    assert(ar.legs == 4);

    // --- 成员布局心智 (示意, ABI 相关) ---
    // Dog:
    //   [vptr][legs][pad?][warm_blooded][pad?][breed string...]
    // Animal* 指向同一起点; 派生成员在基类子对象之后。
    dog.legs = 4;
    dog.warm_blooded = true;
    dog.breed = "Akita";
    assert(as_animal->legs == 4);
    assert(static_cast<Dog*>(as_animal)->breed == "Akita");

    // --- 构造期虚函数: 只看到当前层 ---
    TraceDerived td;
    assert(std::string(td.tag) == "TraceDerived");  // 派生构造结束时 tag 已是派生
    // 构造 Base 子对象时 who() 若被调用会是 TraceBase — 这里用 tag 在两层 ctor 里写入

    // 静态类型截断 vs 指针: 见 object_slicing topic
    Animal copy = dog;  // 切片 (本文件只点到, 专篇在 object_slicing)
    assert(copy.name() == "Animal");
    (void)copy;

    std::cout << "single_inheritance_layout: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/single_inheritance_layout", run>;

}  // namespace

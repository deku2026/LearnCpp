// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A8 typeid)
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : typeid
// Topic id : part6/a/section03/typeid
//
// 要点: typeid(T)/typeid(非多态 glvalue) → 编译期静态类型;
//       typeid(多态 glvalue) → 运行期查 vptr 得最派生 type_info;
//       空指针解引用多态 glvalue → std::bad_typeid;
//       name() 实现定义 (MSVC 可读 / Itanium 常 mangled)。
// 参考: [expr.typeid] <typeinfo>

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <typeinfo>

namespace {

struct Base {
    virtual ~Base() = default;
    virtual int id() const { return 0; }
};

struct Derived : Base {
    int id() const override { return 1; }
};

struct Other : Base {
    int id() const override { return 2; }
};

struct Plain {
    int x = 0;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== typeid ===\n";

    // --- 类型名: 始终静态 ---
    assert(typeid(int) == typeid(int));
    assert(typeid(int) != typeid(double));
    assert(typeid(Base) == typeid(Base));
    std::cout << "  typeid(int).name()  = " << typeid(int).name() << '\n';
    std::cout << "  typeid(Base).name() = " << typeid(Base).name() << '\n';

    // --- 非多态对象: 静态类型 ---
    Plain pl;
    [[maybe_unused]] Plain* ppl = &pl;
    assert(typeid(pl) == typeid(Plain));
    assert(typeid(*ppl) == typeid(Plain));
    assert(typeid(ppl) == typeid(Plain*));  // 指针本身, 不是 *ppl

    // --- 多态: 运行期动态类型 ---
    Derived d;
    [[maybe_unused]] Base& br = d;
    Base* bp = &d;
    assert(typeid(br) == typeid(Derived));
    assert(typeid(*bp) == typeid(Derived));
    assert(typeid(bp) == typeid(Base*));  // 指针的静态类型
    assert(typeid(br) != typeid(Base));
    assert(typeid(d) == typeid(Derived));

    Base alone;
    assert(typeid(alone) == typeid(Base));
    assert(typeid(alone) != typeid(d));

    Other o;
    assert(typeid(o) != typeid(d));
    assert(typeid(static_cast<Base&>(o)) == typeid(Other));

    [[maybe_unused]] const std::type_info& ti1 = typeid(*bp);
    [[maybe_unused]] const std::type_info& ti2 = typeid(Derived);
    assert(ti1 == ti2);
    assert(ti1 != typeid(Base));

    std::cout << "  dynamic *bp name: " << typeid(*bp).name() << '\n';
    std::cout << "  static  Base name: " << typeid(Base).name() << '\n';

    // --- bad_typeid: 对空指针做多态 typeid(*p) ---
    Base* np = nullptr;
    [[maybe_unused]] bool bad = false;
    try {
        (void)typeid(*np);
    } catch (const std::bad_typeid& ex) {
        bad = true;
        std::cout << "  bad_typeid: " << ex.what() << '\n';
    }
    assert(bad);

    // 非多态空指针: 不访问对象, 不抛
    [[maybe_unused]] Plain* nplain = nullptr;
    assert(typeid(*nplain) == typeid(Plain));

    [[maybe_unused]] auto same_dynamic = [](const Base& a, const Base& b) { return typeid(a) == typeid(b); };
    Derived d2;
    assert(same_dynamic(d, d2));
    assert(!same_dynamic(d, alone));
    assert(!same_dynamic(d, o));

    std::cout << "typeid: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section03/typeid", run>;

}  // namespace

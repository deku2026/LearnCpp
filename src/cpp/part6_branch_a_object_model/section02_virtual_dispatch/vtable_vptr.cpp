// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A4 vtable / vptr)
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : vtable_vptr
// Topic id : part6/a/section02/vtable_vptr
//
// 要点: 多态对象含隐藏 vptr → 指向该类 vtable; 虚调用 = 取 vptr + 查槽 + 间接跳;
//       覆盖改写槽位, 未覆盖继承基类槽; 空间开销约一个指针, 时间开销两次访存+间接跳。
// 说明: 确切 vptr 位置 / vtable 条目布局是 ABI 细节 (Itanium vs MSVC), 下面只用
//       标准层行为 + sizeof 实证, 注释描述典型 ABI 机制 (不依赖读 vptr 原始字节)。
// 参考: Itanium C++ ABI; [class.virtual]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

namespace {

struct Base {
    int b_data = 11;
    virtual ~Base() = default;
    virtual std::string f() const { return "Base::f"; }
    virtual std::string g() const { return "Base::g"; }
    virtual int id() const { return 1; }
};

struct Derived : Base {
    int d_data = 22;
    std::string f() const override { return "Derived::f"; }
    // g 未覆盖
    int id() const override { return 2; }
};

struct Other : Base {
    std::string f() const override { return "Other::f"; }
    int id() const override { return 3; }
};

// 无虚函数: 无 vptr
struct Plain {
    int x = 0;
};

static_assert(std::is_polymorphic_v<Base>);
static_assert(std::is_polymorphic_v<Derived>);
static_assert(!std::is_polymorphic_v<Plain>);
static_assert(!std::is_standard_layout_v<Base>);
static_assert(!std::is_trivially_copyable_v<Base>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== A4 vtable / vptr (dynamic dispatch) ===\n";

    // --- 入门: 多态调用看“实际类型” ---
    Derived d;
    Base* p = &d;
    assert(p->f() == "Derived::f");  // 经 vtable 命中 Derived::f
    assert(p->g() == "Base::g");     // 未覆盖 → 基类槽
    assert(p->id() == 2);

    Base alone;
    Base* q = &alone;
    assert(q->f() == "Base::f");
    assert(q->id() == 1);

    // 同一调用点, 不同 vptr → 不同目标
    std::string (*through_base)(const Base*) = [](const Base* b) { return b->f(); };
    (void)through_base;
    assert(through_base(&d) == "Derived::f");
    assert(through_base(&alone) == "Base::f");

    // --- 进阶: 空间 — 虚函数带来约一个指针大小的隐藏成员 ---
    std::cout << "  sizeof(Plain)   =" << sizeof(Plain) << '\n';
    std::cout << "  sizeof(Base)    =" << sizeof(Base) << "  (vptr + b_data + pad)\n";
    std::cout << "  sizeof(Derived) =" << sizeof(Derived) << "  (shared vptr + Base + d_data)\n";

    assert(sizeof(Base) >= sizeof(void*) + sizeof(int));
    assert(sizeof(Derived) >= sizeof(Base));
    // 单继承通常共用一个 vptr, 不因覆盖再多一个
    assert(sizeof(Derived) <= sizeof(void*) + 2 * sizeof(int) + alignof(void*));

    // 对比: 给 Plain 加虚函数会突然变大
    struct SuddenlyVirtual {
        int x = 0;
        virtual void touch() {}
        virtual ~SuddenlyVirtual() = default;
    };
    std::cout << "  sizeof(SuddenlyVirtual)=" << sizeof(SuddenlyVirtual) << " vs Plain=" << sizeof(Plain) << '\n';
    assert(sizeof(SuddenlyVirtual) > sizeof(Plain));

    // --- 专家: 机制注释 + 引用/智能指针同样走虚分派 ---
    // 典型虚调用 (伪代码, Itanium 常把 vptr 放对象开头):
    //   auto vptr = *reinterpret_cast<void***>(obj);
    //   auto fn    = vptr[slot_of_f];
    //   fn(obj);
    // MSVC ABI 细节不同, 但“查表间接跳”相同。
    //
    // Itanium vtable 槽示意 (vptr 指向第一个函数项, 元数据在负偏移):
    //   [-2] offset-to-top
    //   [-1] type_info*
    //   [0]  f
    //   [1]  g
    //   ...

    Other o;
    [[maybe_unused]] const Base& r1 = d;
    [[maybe_unused]] const Base& r2 = o;
    assert(r1.f() == "Derived::f");
    assert(r2.f() == "Other::f");

    auto up = std::make_unique<Derived>();
    [[maybe_unused]] Base* bp = up.get();
    assert(bp->id() == 2);

    // final / 去虚化: 编译器在静态可知类型时可直接调 (优化), 语义仍正确
    Derived stack_d;
    assert(stack_d.f() == "Derived::f");  // 静态类型即 Derived, 可去虚

    std::cout << "  dispatch samples: " << p->f() << ", " << q->g() << '\n';
    std::cout << "vtable_vptr: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/vtable_vptr", run>;

}  // namespace

// LearnCpp topic
// Doc      : 第6部分-支线G · G9.3 std::launder
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : std_launder_when_necessary
// Topic id : part6/g/section04/std_launder_when_necessary
//
// 要点: 在存储上结束旧对象并 placement new 新对象后,
//       旧指针可能需 launder 才能指向新对象 (const/引用成员等场景)。
// 参考: [ptr.launder] P0137

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <new>

namespace {

struct WithConst {
    const int id;
    explicit WithConst(int i) : id(i) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G9 std::launder when necessary ===\n";

    alignas(WithConst) unsigned char buf[sizeof(WithConst)];
    auto* p = new (buf) WithConst(1);
    assert(p->id == 1);

    // 结束生命周期并在同一存储创建新对象
    p->~WithConst();
    auto* q = new (buf) WithConst(2);
    assert(q->id == 2);

    // 通过新指针访问 — 始终正确
    // 若仍持有指向旧对象的指针 p, 对含 const 成员的类型,
    // 标准要求 std::launder(p) 才指向新对象:
    assert(std::launder(reinterpret_cast<WithConst*>(buf))->id == 2);

    // 平凡无 const 成员时编译器通常"能看见" placement, 但仍推荐清晰生命周期

    // 容器场景: 自己管理的 slot 复用
    struct Slot {
        int v;
    };
    alignas(Slot) unsigned char slot[sizeof(Slot)];
    Slot* s = new (slot) Slot{10};
    s->~Slot();
    s = new (slot) Slot{20};
    assert(std::launder(s)->v == 20);

    std::cout << "  launder: provenance/lifetime bridge after storage reuse\n";
    std::cout << "  not a cast to 'ignore const' on the same object\n";
    std::cout << "std_launder_when_necessary: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/std_launder_when_necessary", run>;

}  // namespace

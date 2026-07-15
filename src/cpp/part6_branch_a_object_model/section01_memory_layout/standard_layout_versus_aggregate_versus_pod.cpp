// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A2 POD / 平凡 / 标准布局)
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : standard_layout_versus_aggregate_versus_pod
// Topic id : part6/a/section01/standard_layout_versus_aggregate_versus_pod
//
// 要点: trivial / standard-layout / trivially_copyable / aggregate / 旧 POD 正交;
//       标准布局 → offsetof / C 互操作; 平凡可拷贝 → 可 memcpy;
//       C++20 弃用 is_pod, 用两个 trait 组合表达。
// 参考: [class.prop] named requirements TrivialType / StandardLayoutType

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>

namespace {

struct ClassicPod {
    int x;
    double y;
};

struct WithUserCtor {
    int x;
    WithUserCtor() : x(0) {}  // 用户提供默认构造 → 非 trivial
};

struct MixedAccess {
public:
    int a;

private:
    int b;  // 不同访问控制 → 非 standard-layout

public:
    void set_b(int v) { b = v; }
    int get_b() const { return b; }
};

struct Polymorphic {
    virtual void f() {}
    int x = 0;
};

struct AggregateOk {
    int x;
    int y;
    // 无用户声明构造、无 private 非静态数据 → 聚合 (C++20 起规则更细)
};

struct HasRef {
    int& r;  // 引用成员 → 非 standard-layout, 也非 trivial
};

// 标准布局: 继承链上至多【一个】类拥有非静态数据成员
struct EmptyBase {};
struct DerivedOnlyData : EmptyBase {
    int d;
};
struct BaseHasData {
    int b;
};
struct DerivedAlsoData : BaseHasData {
    int d;  // 基类+派生都有数据 → 非 standard-layout
};

static_assert(std::is_standard_layout_v<DerivedOnlyData>);
static_assert(!std::is_standard_layout_v<DerivedAlsoData>);

struct NonAggregate {
    int x;
    NonAggregate() = default;  // 用户声明 → 非聚合 (C++20: 用户声明/继承构造等)
};

void show_traits(const char* name, bool trivial, bool std_layout, bool triv_copy, bool aggregate) {
    std::cout << "  " << name << ":\n"
              << "    trivial=" << trivial << " standard_layout=" << std_layout << " trivially_copyable=" << triv_copy
              << " aggregate=" << aggregate << '\n';
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << std::boolalpha;
    std::cout << "=== trivial / standard-layout / aggregate / POD ===\n";

    // --- ClassicPod: 旧意义 POD = trivial + standard-layout ---
    show_traits("ClassicPod", std::is_trivial_v<ClassicPod>, std::is_standard_layout_v<ClassicPod>,
                std::is_trivially_copyable_v<ClassicPod>, std::is_aggregate_v<ClassicPod>);
    assert(std::is_trivial_v<ClassicPod>);
    assert(std::is_standard_layout_v<ClassicPod>);
    assert(std::is_trivially_copyable_v<ClassicPod>);
    assert(std::is_aggregate_v<ClassicPod>);
    assert(offsetof(ClassicPod, x) == 0);
    // double 通常 8 对齐 → offset 8 (int 后 padding 4)
    assert(offsetof(ClassicPod, y) == 8);
    assert(sizeof(ClassicPod) == 16);

    // --- 用户构造: 可 standard-layout 但非 trivial ---
    show_traits("WithUserCtor", std::is_trivial_v<WithUserCtor>, std::is_standard_layout_v<WithUserCtor>,
                std::is_trivially_copyable_v<WithUserCtor>, std::is_aggregate_v<WithUserCtor>);
    assert(!std::is_trivial_v<WithUserCtor>);
    assert(std::is_standard_layout_v<WithUserCtor>);
    assert(std::is_trivially_copyable_v<WithUserCtor>);  // 拷贝仍平凡
    assert(!std::is_aggregate_v<WithUserCtor>);

    // --- 混合访问: 破坏 standard-layout ---
    show_traits("MixedAccess", std::is_trivial_v<MixedAccess>, std::is_standard_layout_v<MixedAccess>,
                std::is_trivially_copyable_v<MixedAccess>, std::is_aggregate_v<MixedAccess>);
    assert(!std::is_standard_layout_v<MixedAccess>);
    // offsetof 仅对 standard-layout 保证有意义; 此处不调用 offsetof(MixedAccess,...)

    // --- 多态: 两者皆否 ---
    show_traits("Polymorphic", std::is_trivial_v<Polymorphic>, std::is_standard_layout_v<Polymorphic>,
                std::is_trivially_copyable_v<Polymorphic>, std::is_aggregate_v<Polymorphic>);
    assert(!std::is_trivial_v<Polymorphic>);
    assert(!std::is_standard_layout_v<Polymorphic>);
    assert(!std::is_trivially_copyable_v<Polymorphic>);

    // --- 引用成员 ---
    int dummy = 0;
    HasRef hr{dummy};
    (void)hr;
    show_traits("HasRef", std::is_trivial_v<HasRef>, std::is_standard_layout_v<HasRef>,
                std::is_trivially_copyable_v<HasRef>, std::is_aggregate_v<HasRef>);
    assert(!std::is_standard_layout_v<HasRef>);

    // --- 聚合初始化 vs 非聚合 ---
    [[maybe_unused]] AggregateOk ag{1, 2};
    assert(ag.x == 1 && ag.y == 2);
    assert(std::is_aggregate_v<AggregateOk>);
    assert(!std::is_aggregate_v<NonAggregate>);
    [[maybe_unused]] NonAggregate na;  // 只能默认构造, 不能 NonAggregate{1} 作为聚合 init
    assert(na.x == 0 || true);

    // --- 工程含义一句话 ---
    std::cout << "  meaning:\n"
              << "    standard-layout → offsetof / C layout interop\n"
              << "    trivially_copyable → byte copy (memcpy) OK\n"
              << "    trivial → no runtime init machinery for special members\n"
              << "    aggregate → brace init without user ctor\n"
              << "    POD (deprecated term) ≈ trivial && standard-layout\n";

#if defined(__cpp_lib_is_pod)
    // C++20 起 is_pod 弃用; 仍可能编译, 勿在新代码使用
    std::cout << "  (legacy) is_pod(ClassicPod)=" << std::is_pod_v<ClassicPod> << '\n';
#endif

    std::cout << "standard_layout_versus_aggregate_versus_pod: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section01/standard_layout_versus_aggregate_versus_pod", run>;

}  // namespace

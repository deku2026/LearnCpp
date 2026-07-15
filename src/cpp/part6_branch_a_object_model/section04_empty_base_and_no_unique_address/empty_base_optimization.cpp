// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A3 EBO)
// Stage    : part6_branch_a_object_model
// Section  : section04_empty_base_and_no_unique_address
// Item     : empty_base_optimization
// Topic id : part6/a/section04/empty_base_optimization
//
// 要点: 独立空类 sizeof >= 1 (唯一地址); 作基类时常不占空间 (EBO);
//       典型用途: 空分配器/删除器/策略类压缩进 vector / unique_ptr;
//       两个相同空基类子对象仍需不同地址 (不能完全叠在同一点的限制因 ABI 而异)。
// 参考: [class.derived] EBO; libc++/MSVC STL compressed_pair 思想

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

namespace {

struct Empty {};

struct EmptyWithStatic {
    static int count;
    void bump() { ++count; }
};
int EmptyWithStatic::count = 0;

struct HoldsInt : Empty {
    int value = 0;
};

struct HoldsIntMember {
    Empty e;
    int value = 0;
};

// 策略模式: 空策略作基类 → 不增体积
struct AddPolicy {
    static int combine(int a, int b) { return a + b; }
};

struct MulPolicy {
    static int combine(int a, int b) { return a * b; }
};

template <typename Policy>
struct Calculator : private Policy {
    int eval(int a, int b) const { return Policy::combine(a, b); }
};

// 模拟 unique_ptr 压缩: 空删除器与指针同驻
template <typename T, typename Deleter>
struct CompressedPtr : private Deleter {
    T* p = nullptr;
    CompressedPtr() = default;
    explicit CompressedPtr(T* ptr) : p(ptr) {}
    ~CompressedPtr() {
        if (p) {
            Deleter::operator()(p);
        }
    }
    CompressedPtr(const CompressedPtr&) = delete;
    CompressedPtr& operator=(const CompressedPtr&) = delete;
};

struct EmptyDelete {
    template <typename U>
    void operator()(U* q) const {
        delete q;
    }
};

struct StatefulDelete {
    int freed = 0;
    template <typename U>
    void operator()(U* q) const {
        delete q;
        // 演示: 有状态删除器无法 EBO 到零开销 (sizeof 会含 int)
    }
};

// 有状态删除器用成员存储
template <typename T>
struct PtrWithStateDeleter {
    T* p = nullptr;
    StatefulDelete del{};
    ~PtrWithStateDeleter() {
        if (p) {
            delete p;
            // del.freed 不可在 const operator 里改 — 仅示意布局
        }
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== empty base optimization (EBO) ===\n";

    // --- 入门: 独立空类至少 1 字节 ---
    std::cout << "  sizeof(Empty)=" << sizeof(Empty) << '\n';
    assert(sizeof(Empty) >= 1);

    Empty a, b;
    assert(&a != &b);  // 不同对象不同地址

    // --- EBO: 空基类 + int ≈ sizeof(int) ---
    std::cout << "  sizeof(HoldsInt : Empty)=" << sizeof(HoldsInt) << '\n';
    std::cout << "  sizeof(HoldsIntMember {Empty,int})=" << sizeof(HoldsIntMember) << '\n';
    assert(sizeof(HoldsInt) == sizeof(int));
    assert(sizeof(HoldsIntMember) > sizeof(int));  // 成员 Empty 至少占 1 + padding

    static_assert(std::is_empty_v<Empty>);
    static_assert(std::is_empty_v<EmptyWithStatic>);  // 仅静态成员仍可 empty
    static_assert(std::is_empty_v<AddPolicy>);

    // --- 策略类零开销 ---
    Calculator<AddPolicy> add;
    Calculator<MulPolicy> mul;
    assert(add.eval(2, 3) == 5);
    assert(mul.eval(2, 3) == 6);
    std::cout << "  sizeof(Calculator<AddPolicy>)=" << sizeof(add) << '\n';
    assert(sizeof(add) == 1 || sizeof(add) == sizeof(Empty));  // 空类对象至少 1
    // 无数据成员的类 sizeof>=1; 若再继承 Empty 策略, 仍可保持最小

    // --- compressed pointer 示意 ---
    {
        CompressedPtr<int, EmptyDelete> cp(new int(7));
        assert(*cp.p == 7);
        std::cout << "  sizeof(CompressedPtr<int,EmptyDelete>)=" << sizeof(cp) << " (hope == sizeof(void*))\n";
        assert(sizeof(cp) == sizeof(void*));
    }
    std::cout << "  sizeof(PtrWithStateDeleter<int>)=" << sizeof(PtrWithStateDeleter<int>) << " (pointer + state)\n";
    assert(sizeof(PtrWithStateDeleter<int>) > sizeof(void*));

    // 标准库对照: std::unique_ptr 对空删除器通常 sizeof == sizeof(T*)
    std::unique_ptr<int> up(new int(1));
    std::cout << "  sizeof(unique_ptr<int>)=" << sizeof(up) << '\n';
    assert(sizeof(up) == sizeof(void*));

    // 空基类仍可调用 (静态接口/非静态空操作)
    EmptyWithStatic ews;
    ews.bump();
    assert(EmptyWithStatic::count >= 1);

    HoldsInt hi;
    hi.value = 42;
    Empty* as_empty = &hi;  // 基类子对象地址
    assert(static_cast<void*>(as_empty) == static_cast<void*>(&hi));

    std::cout << "empty_base_optimization: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section04/empty_base_optimization", run>;

}  // namespace

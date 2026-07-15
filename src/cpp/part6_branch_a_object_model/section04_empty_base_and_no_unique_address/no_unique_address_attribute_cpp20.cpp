// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A3 [[no_unique_address]])
// Stage    : part6_branch_a_object_model
// Section  : section04_empty_base_and_no_unique_address
// Item     : no_unique_address_attribute_cpp20
// Topic id : part6/a/section04/no_unique_address_attribute_cpp20
//
// 要点: C++20 将 EBO 扩展到成员 — 空成员可不占独立字节;
//       属性是“允许优化”非强制; MSVC ABI (含 clang-cl) 用 [[msvc::no_unique_address]];
//       同类型多个空成员仍需不同地址 → 每个至少 1 字节。
// 参考: [dcl.attr.nouniqueaddr] cppreference no_unique_address

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>

namespace {

// MSVC ABI: 标准 [[no_unique_address]] 被忽略以保持布局兼容;
// 需 [[msvc::no_unique_address]] (MSVC 与 clang-cl 皆然)。
#if defined(_MSC_VER)
#define LEARN_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define LEARN_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

struct Empty {};
struct Empty2 {};

struct WithoutAttr {
    Empty e;
    int x;
};

struct WithAttr {
    LEARN_NO_UNIQUE_ADDRESS Empty e;
    int x;
};

// 双空成员: 类型相同 → 地址必须不同
struct TwoSameEmpty {
    LEARN_NO_UNIQUE_ADDRESS Empty a;
    LEARN_NO_UNIQUE_ADDRESS Empty b;
    char c;
};

// 类型不同: 更可能完全重叠优化
struct TwoDiffEmpty {
    LEARN_NO_UNIQUE_ADDRESS Empty a;
    LEARN_NO_UNIQUE_ADDRESS Empty2 b;
    int x;
};

// 应用: 压缩“策略 + 数据”
template <typename Policy>
struct Compact {
    LEARN_NO_UNIQUE_ADDRESS Policy policy;
    int data;
    int apply(int v) const { return policy.map(v); }
};

struct TimesTwo {
    int map(int v) const { return v * 2; }
};

static_assert(std::is_empty_v<Empty>);
static_assert(std::is_empty_v<TimesTwo>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [[no_unique_address]] / [[msvc::no_unique_address]] ===\n";

    std::cout << "  sizeof(WithoutAttr)=" << sizeof(WithoutAttr) << '\n';
    std::cout << "  sizeof(WithAttr)   =" << sizeof(WithAttr) << '\n';
    assert(sizeof(WithoutAttr) > sizeof(int));
    assert(sizeof(WithAttr) <= sizeof(WithoutAttr));

    if (sizeof(WithAttr) == sizeof(int)) {
        std::cout << "  empty member overlapped (optimization applied)\n";
    } else {
        std::cout << "  note: implementation kept unique storage for empty member\n";
    }

    WithAttr w{};
    w.x = 7;
    w.e = Empty{};
    assert(w.x == 7);
    std::cout << "  &w.e=" << static_cast<const void*>(&w.e) << " &w.x=" << static_cast<const void*>(&w.x) << '\n';

    [[maybe_unused]] TwoSameEmpty tse{};
    assert(static_cast<const void*>(&tse.a) != static_cast<const void*>(&tse.b));
    std::cout << "  sizeof(TwoSameEmpty)=" << sizeof(TwoSameEmpty) << '\n';

    TwoDiffEmpty tde{};
    tde.x = 1;
    std::cout << "  sizeof(TwoDiffEmpty)=" << sizeof(TwoDiffEmpty) << " &a=" << static_cast<const void*>(&tde.a)
              << " &b=" << static_cast<const void*>(&tde.b) << '\n';
    assert(tde.x == 1);

    Compact<TimesTwo> c{};
    c.data = 21;
    assert(c.apply(c.data) == 42);
    std::cout << "  sizeof(Compact<TimesTwo>)=" << sizeof(c) << '\n';
    assert(sizeof(c) >= sizeof(int));

    // 与 EBO 对照: 不能/不想继承时 (final 策略、组合设计) 用 no_unique_address
    std::cout << "no_unique_address_attribute_cpp20: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section04/no_unique_address_attribute_cpp20", run>;

}  // namespace

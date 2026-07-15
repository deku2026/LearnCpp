// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A1 对象内存布局)
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : member_layout_alignment_padding
// Topic id : part6/a/section01/member_layout_alignment_padding
//
// 要点: 非静态数据成员按声明顺序排列; 对齐要求导致 padding;
//       结构体对齐 = 成员最大对齐; sizeof 向上取整到对齐倍数;
//       编译器不自动重排成员 — 手动把大对齐成员放前面可省空间。
// 参考: [class.mem] [basic.align] cppreference object / alignof / offsetof

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace {

// 典型 padding: char 后为 int 补 3 字节; 末尾为整体对齐再补 2。
struct SparseLayout {
    char c;   // offset 0, size 1
    int i;    // offset 4, size 4  (pad 3 after c)
    short s;  // offset 8, size 2
    // trailing pad 2 → sizeof 12, alignof 4
};

// 同一组成员, 大对齐在前 → 更少 padding。
struct DenseLayout {
    int i;    // 0
    short s;  // 4
    char c;   // 6
    // trailing pad 1 → sizeof 8
};

// 混排浪费更多空间。
struct WastefulLayout {
    char c;  // 0
    int i;   // 4
    char d;  // 8
    // trailing pad 3 → sizeof 12
};

// alignas 可强制更大对齐 (如 SIMD / 缓存行)。
struct alignas(16) OverAligned {
    int x;
};

// 嵌套: 内层对齐要求会“渗透”到外层。
struct Nested {
    char c;
    SparseLayout inner;  // alignof 4 → c 后 pad
};

static_assert(std::is_standard_layout_v<SparseLayout>);
static_assert(offsetof(SparseLayout, c) == 0);
static_assert(offsetof(SparseLayout, i) == 4);
static_assert(offsetof(SparseLayout, s) == 8);
static_assert(sizeof(SparseLayout) == 12);
static_assert(alignof(SparseLayout) == 4);

static_assert(offsetof(DenseLayout, i) == 0);
static_assert(offsetof(DenseLayout, s) == 4);
static_assert(offsetof(DenseLayout, c) == 6);
static_assert(sizeof(DenseLayout) == 8);

static_assert(sizeof(WastefulLayout) == 12);
static_assert(alignof(OverAligned) == 16);
static_assert(sizeof(OverAligned) % 16 == 0);

void print_layout(const char* name, std::size_t sz, std::size_t al) {
    std::cout << "  " << name << ": sizeof=" << sz << " alignof=" << al << '\n';
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== A1 member layout / alignment / padding ===\n";

    // --- 入门: 手算 vs 实测 ---
    print_layout("SparseLayout {char,int,short}", sizeof(SparseLayout), alignof(SparseLayout));
    print_layout("DenseLayout  {int,short,char}", sizeof(DenseLayout), alignof(DenseLayout));
    print_layout("WastefulLayout {char,int,char}", sizeof(WastefulLayout), alignof(WastefulLayout));

    assert(sizeof(DenseLayout) < sizeof(WastefulLayout));
    assert(sizeof(DenseLayout) + 4 == sizeof(WastefulLayout));  // 省 4 字节

    // --- 进阶: 指针算术与 padding 空洞 ---
    SparseLayout a{};
    a.c = 'Z';
    a.i = 0x11223344;
    a.s = 0x55AA;

    [[maybe_unused]] const auto* base = reinterpret_cast<const unsigned char*>(&a);
    // i 不在 &c+1, 而在 offset 4
    assert(reinterpret_cast<const void*>(&a.i) == static_cast<const void*>(base + 4));
    assert(reinterpret_cast<const void*>(&a.s) == static_cast<const void*>(base + 8));

    // 偏移差 - 成员大小 = 中间 padding
    const auto pad_after_c = offsetof(SparseLayout, i) - offsetof(SparseLayout, c) - sizeof(char);
    const auto trailing = sizeof(SparseLayout) - (offsetof(SparseLayout, s) + sizeof(short));
    std::cout << "  pad after char: " << pad_after_c << "  trailing pad: " << trailing << '\n';
    assert(pad_after_c == 3);
    assert(trailing == 2);

    // --- 专家: alignas / 数组步长 / 嵌套 ---
    [[maybe_unused]] OverAligned oa{};
    assert(reinterpret_cast<std::uintptr_t>(&oa) % 16 == 0);
    // 数组元素间距 = sizeof, 已含对齐
    OverAligned arr[2]{};
    [[maybe_unused]] const auto stride =
        reinterpret_cast<const char*>(&arr[1]) - reinterpret_cast<const char*>(&arr[0]);
    assert(static_cast<std::size_t>(stride) == sizeof(OverAligned));
    assert(stride % 16 == 0);

    [[maybe_unused]] Nested n{};
    assert(offsetof(Nested, inner) % alignof(SparseLayout) == 0);
    assert(sizeof(Nested) >= sizeof(char) + sizeof(SparseLayout));
    print_layout("Nested {char, SparseLayout}", sizeof(Nested), alignof(Nested));

    // 标量常见对齐 (实现定义, 在 LP64/LLP64 上通常如下)
    std::cout << "  alignof(char)=" << alignof(char) << " int=" << alignof(int) << " double=" << alignof(double)
              << " void*=" << alignof(void*) << '\n';

    std::cout << "member_layout_alignment_padding: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section01/member_layout_alignment_padding", run>;

}  // namespace

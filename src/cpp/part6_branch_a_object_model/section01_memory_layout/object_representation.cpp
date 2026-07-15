// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A1/A2 对象表示)
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : object_representation
// Topic id : part6/a/section01/object_representation
//
// 要点: 对象表示 = 对象占用的字节序列; 值表示是其中决定值的比特;
//       padding 字节是对象表示的一部分但通常不参与“值相等”;
//       平凡可拷贝类型可按字节 memcpy (值表示可移植性仍受端序/布局约束)。
// 参考: [basic.types.general] object representation / value representation

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <span>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

struct Point2 {
    std::int32_t x;
    std::int32_t y;
};

struct WithPad {
    char tag;
    int value;  // 3 字节 padding 在 tag 后
};

static_assert(std::is_trivially_copyable_v<Point2>);
static_assert(std::is_trivially_copyable_v<WithPad>);
static_assert(sizeof(Point2) == 8);
static_assert(offsetof(WithPad, value) == 4);

void dump_bytes(std::string_view label, std::span<const std::byte> bytes) {
    std::cout << "  " << label << " (" << bytes.size() << " B):";
    for (std::byte b : bytes) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(b);
    }
    std::cout << std::dec << '\n';
}

// 通过 unsigned char 遍历是标准允许查看对象表示的方式 (C++20+ 也可用 std::byte)。
std::vector<std::byte> object_bytes(const auto& obj) {
    const auto* p = reinterpret_cast<const std::byte*>(&obj);
    return std::vector<std::byte>(p, p + sizeof(obj));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== object representation (byte-level) ===\n";

    // --- 入门: 连续存储与按字节拷贝 ---
    Point2 a{0x01020304, 0x05060708};
    auto bytes_a = object_bytes(a);
    dump_bytes("Point2 a", bytes_a);

    Point2 b{};
    std::memcpy(&b, &a, sizeof(Point2));  // 平凡可拷贝 → 合法
    assert(b.x == a.x && b.y == a.y);

    // 第一个成员地址等于对象地址 (标准布局保证)
    assert(static_cast<const void*>(&a) == static_cast<const void*>(&a.x));

    // --- 进阶: padding 也在对象表示里 ---
    WithPad w1{'A', 7};
    WithPad w2{'A', 7};
    // 值相同, 但 padding 字节内容未规定 — 勿用 memcmp 当“值相等”
    auto bw1 = object_bytes(w1);
    auto bw2 = object_bytes(w2);
    dump_bytes("WithPad w1", bw1);
    assert(bw1.size() == sizeof(WithPad));
    assert(bw1[0] == std::byte{'A'});
    // value 在 offset 4
    [[maybe_unused]] const auto* vbytes = reinterpret_cast<const std::byte*>(&w1.value);
    assert(std::memcmp(vbytes, bw1.data() + 4, sizeof(int)) == 0);

    // 安全比较值: 比成员, 或只比较值表示相关区域
    assert(w1.tag == w2.tag && w1.value == w2.value);

    // --- 专家: 值表示 vs 对象表示; 多态对象不可 memcpy ---
    // 布尔: 值只有 true/false, 但对象表示可能多字节/实现定义
    bool flag = true;
    auto bf = object_bytes(flag);
    dump_bytes("bool true", bf);
    assert(bf.size() == sizeof(bool));

    struct Poly {
        virtual ~Poly() = default;
        int payload = 1;
    };
    static_assert(!std::is_trivially_copyable_v<Poly>);
    // 禁止: std::memcpy 多态对象会弄坏 vptr / 生命周期规则
    Poly p1;
    Poly p2;
    // 正确: 用拷贝/赋值 (此处默认不可拷贝因虚析构? 默认拷贝仍生成但切片风险)
    // Poly 有虚析构 → 仍可能 trivial? 不: 有虚函数 → 非 trivially copyable
    assert(sizeof(Poly) >= sizeof(void*) + sizeof(int));  // 至少 vptr + int

    // 字节视图与端序: 小端机器上 0x01020304 低字节在低地址
    std::uint32_t word = 0x01020304u;
    auto wb = object_bytes(word);
    dump_bytes("uint32 0x01020304", wb);
#if defined(_WIN32) || defined(__LITTLE_ENDIAN__) || \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    assert(wb[0] == std::byte{0x04});
    assert(wb[3] == std::byte{0x01});
    std::cout << "  (little-endian observed)\n";
#endif

    std::cout << "object_representation: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section01/object_representation", run>;

}  // namespace

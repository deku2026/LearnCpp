// Topic    : 枚举底层类型、sizeof、前向声明
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 8.3
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : enum_underlying_type
// Topic id : part2/stage02/section08/enum_underlying_type
// Refs     : https://en.cppreference.com/cpp/language/enum
//            https://en.cppreference.com/cpp/types/underlying_type
//            ISO [dcl.enum]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace {

// 指定底层类型：固定大小（协议 / 省内存 / ABI）
enum class Status : std::uint8_t { Ok, Warn, Error };
enum class Code : int;  // ✅ 前向声明：底层类型已知

enum class Code : int { Success = 0, Fail = -1, Retry = 2 };

// 传统 enum 指定底层后也可前向声明
enum Legacy : int;
enum Legacy : int { L0, L1 };

// 未指定底层的传统 enum —— 不能前向声明（大小未知）
// enum Opaque;  // ❌
enum Opaque { O1 = 1, O2 = 2 };

// 位标志常用固定宽度
enum class Perm : std::uint8_t {
    None = 0,
    Read = 1 << 0,
    Write = 1 << 1,
    Exec = 1 << 2,
};

constexpr Perm operator|(Perm a, Perm b) {
    return static_cast<Perm>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
}
[[maybe_unused]] constexpr bool any(Perm p, Perm mask) {
    return (static_cast<std::uint8_t>(p) & static_cast<std::uint8_t>(mask)) != 0;
}

// 使用前向声明的 Code
[[maybe_unused]] int severity(Code c) {
    return static_cast<int>(c);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [enum_underlying_type] 入门：固定底层 + sizeof ===\n";
    {
        static_assert(std::is_same_v<std::underlying_type_t<Status>, std::uint8_t>);
        static_assert(sizeof(Status) == 1);
        static_assert(std::is_same_v<std::underlying_type_t<Code>, int>);
        static_assert(sizeof(Code) == sizeof(int));

        [[maybe_unused]] Status s = Status::Warn;
        assert(static_cast<std::uint8_t>(s) == 1);
        std::cout << "[intro] Status:uint8_t sizeof=" << sizeof(Status) << '\n';
    }

    std::cout << "=== 进阶：前向声明前提是「底层类型已知」===\n";
    {
        assert(severity(Code::Fail) == -1);
        assert(static_cast<int>(Legacy::L1) == 1);

        // scoped 默认底层 int；unscoped 未指定时由实现选
        static_assert(std::is_same_v<std::underlying_type_t<Opaque>, std::underlying_type_t<Opaque>>);
        std::cout << "[advanced] forward declare only with fixed underlying type\n";
    }

    std::cout << "=== 专家：位标志需自写运算符（无隐式 int）===\n";
    {
        [[maybe_unused]] Perm p = Perm::Read | Perm::Write;
        assert(any(p, Perm::Read));
        assert(any(p, Perm::Write));
        assert(!any(p, Perm::Exec));
        assert(sizeof(p) == 1);

        // 网络/文件格式：固定宽度避免「实现选 int 宽度」差异
        [[maybe_unused]] std::uint8_t wire = static_cast<std::uint8_t>(Status::Error);
        assert(wire == 2);

        std::cout << "[expert] fixed underlying helps ABI, packing, bitflags\n";
    }

    std::cout << "[enum_underlying_type] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/enum_underlying_type", run>;

}  // namespace

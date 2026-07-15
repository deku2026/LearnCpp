// Topic    : C++23 std::to_underlying
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 8.5
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : to_underlying_cpp23
// Topic id : part2/stage02/section08/to_underlying_cpp23
// Refs     : https://en.cppreference.com/cpp/utility/to_underlying
//            提案 P1682

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <iostream>
#include <type_traits>
#include <utility>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

enum class Status : int { Ok = 0, Warn = 1, Error = 2 };
enum class Tiny : std::uint8_t { A = 10, B = 20 };
enum Unscoped { U0 = 5, U1 = 6 };

// 旧写法封装
template <class E>
constexpr auto old_to_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [to_underlying_cpp23] 入门：简洁取底层整型 ===\n";
    {
        Status s = Status::Warn;
        // 旧：static_cast<std::underlying_type_t<Status>>(s)
        int code = std::to_underlying(s);
        assert(code == 1);
        assert(std::to_underlying(Status::Error) == 2);

        static_assert(std::is_same_v<decltype(std::to_underlying(s)), int>);
        std::cout << "[intro] to_underlying(Status::Warn)=" << code << '\n';
    }

    std::cout << "=== 进阶：底层类型跟随 enum；与旧写法等价 ===\n";
    {
        Tiny t = Tiny::B;
        auto u = std::to_underlying(t);
        static_assert(std::is_same_v<decltype(u), std::uint8_t>);
        assert(u == 20);
        assert(old_to_underlying(t) == std::to_underlying(t));

        // unscoped 同样可用
        assert(std::to_underlying(U1) == 6);

        // 日志 / 序列化 / 与 C API 交互时的惯用出口
        [[maybe_unused]] const int wire = std::to_underlying(Status::Ok);
        assert(wire == 0);
        std::cout << "[advanced] return type is underlying_type_t<E>\n";
    }

    std::cout << "=== 专家：feature 宏；位标志取值 ===\n";
    {
#if defined(__cpp_lib_to_underlying)
        std::cout << "[expert] __cpp_lib_to_underlying=" << __cpp_lib_to_underlying << '\n';
#else
        std::cout << "[expert] __cpp_lib_to_underlying not defined\n";
#endif
        enum class Flag : std::uint16_t {
            None = 0,
            Read = 1,
            Write = 2,
            Exec = 4,
        };
        [[maybe_unused]] const auto bits =
            static_cast<std::uint16_t>(std::to_underlying(Flag::Read) | std::to_underlying(Flag::Exec));
        assert(bits == 5);
        std::cout << "[expert] prefer to_underlying over ad-hoc static_cast\n";
    }

    std::cout << "[to_underlying_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/to_underlying_cpp23", run>;

}  // namespace

// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : to_underlying_cpp23
// Topic id : part2/stage02/section08/to_underlying_cpp23
//
// Covers: std::to_underlying (C++23) vs static_cast fallback

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <type_traits>
#include <utility>
#include <version>

namespace {

enum class Status : std::uint16_t { Ok = 0, Warn = 1, Error = 2 };
enum class SignedCode : int { Neg = -1, Zero = 0, Pos = 1 };

void demo_basics() {
    Status s = Status::Error;
#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
    auto u = std::to_underlying(s);
#else
    auto u = static_cast<std::underlying_type_t<Status>>(s);
#endif
    static_assert(std::is_same_v<decltype(u), std::uint16_t>);
    LEARN_CHECK(u == 2);
}

void demo_intermediate() {
    SignedCode c = SignedCode::Neg;
#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
    auto u = std::to_underlying(c);
#else
    auto u = static_cast<std::underlying_type_t<SignedCode>>(c);
#endif
    LEARN_CHECK(u == -1);

    // Round-trip
    auto back = static_cast<SignedCode>(u);
    LEARN_CHECK(back == SignedCode::Neg);
}

void demo_expert() {
    auto as_underlying = [](auto e) {
#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
        return std::to_underlying(e);
#else
        using E = decltype(e);
        return static_cast<std::underlying_type_t<E>>(e);
#endif
    };

    LEARN_CHECK(as_underlying(Status::Ok) == 0);
    LEARN_CHECK(as_underlying(Status::Warn) == 1);
    LEARN_CHECK(as_underlying(SignedCode::Pos) == 1);

    // Handy for serialization buffers
    std::uint16_t wire = static_cast<std::uint16_t>(as_underlying(Status::Error));
    LEARN_CHECK(wire == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/to_underlying_cpp23", run>;

}  // namespace

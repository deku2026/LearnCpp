// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : enum_underlying_type
// Topic id : part2/stage02/section08/enum_underlying_type
//
// Covers: fixed underlying type, std::underlying_type, size control

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <type_traits>

namespace {

enum class Tiny : std::uint8_t { A = 0, B = 1, C = 255 };
enum class Wide : std::int64_t { Min = -1, Max = 1'000'000'000'000LL };
enum Legacy { LegacyZero, LegacyOne };  // underlying implementation-defined

void demo_basics() {
    Tiny t = Tiny::C;
    assert(static_cast<std::uint8_t>(t) == 255);
    static_assert(sizeof(Tiny) == 1);

    Wide w = Wide::Max;
    assert(static_cast<std::int64_t>(w) == 1'000'000'000'000LL);
    static_assert(sizeof(Wide) == 8);
}

void demo_intermediate() {
    using UTiny = std::underlying_type_t<Tiny>;
    using UWide = std::underlying_type_t<Wide>;
    static_assert(std::is_same_v<UTiny, std::uint8_t>);
    static_assert(std::is_same_v<UWide, std::int64_t>);

    UTiny raw = static_cast<UTiny>(Tiny::B);
    assert(raw == 1);

    // Fixed underlying type enables forward declaration of enum class in other TUs.
    static_assert(std::is_same_v<std::underlying_type_t<Tiny>, unsigned char> ||
                  std::is_same_v<std::underlying_type_t<Tiny>, std::uint8_t>);
}

void demo_expert() {
    // Legacy unscoped enum still has some underlying type
    using UL = std::underlying_type_t<Legacy>;
    static_assert(std::is_integral_v<UL>);
    assert(static_cast<UL>(LegacyOne) == 1);

    // Arithmetic on underlying values after cast
    auto sum = static_cast<int>(Tiny::A) + static_cast<int>(Tiny::B);
    assert(sum == 1);

    // Choose width deliberately for ABI / serialization
    enum class WireCode : std::uint16_t { Ok = 200, NotFound = 404 };
    static_assert(sizeof(WireCode) == 2);
    assert(static_cast<std::uint16_t>(WireCode::NotFound) == 404);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/enum_underlying_type", run>;

}  // namespace

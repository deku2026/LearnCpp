// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : object_representation
// Topic id : part6/a/section01/object_representation
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/a/section01/object_representation";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr std::uint32_t value = 0x11223344U;
    const auto bytes = std::bit_cast<std::array<std::byte, sizeof(value)>>(value);
    const auto round_trip = std::bit_cast<std::uint32_t>(bytes);
    LEARN_EXPECT_EQ(checks, round_trip, value);
    LEARN_EXPECT_EQ(checks, bytes.size(), sizeof(value));
    // Padding bytes, endianness and multiple representations mean raw bytes are not a portable wire format.
    LEARN_EXPECT(checks, std::is_trivially_copyable_v<std::uint32_t>);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section01/object_representation", run>;

}  // namespace

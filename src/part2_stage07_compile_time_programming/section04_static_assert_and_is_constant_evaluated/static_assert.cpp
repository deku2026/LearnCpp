// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section04_static_assert_and_is_constant_evaluated
// Item     : static_assert
// Topic id : part2/stage07/section04/static_assert
// References: N4950 [dcl.pre], [temp.res].

#include "learn/example_support.hpp"

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace {

constexpr bool is_power_of_two(std::size_t value) noexcept {
    return value != 0 && (value & (value - 1)) == 0;
}

struct PacketHeader {
    std::uint32_t payload_size;
    std::uint16_t kind;
    std::uint16_t flags;
};

template <class Type>
struct BinarySerializer {
    static_assert(std::is_trivially_copyable_v<Type>, "raw byte serialization requires a trivially copyable type");
    static constexpr std::size_t width = sizeof(Type);
};

static_assert(is_power_of_two(64));  // Message became optional in C++17.
static_assert(!is_power_of_two(48), "48 is not a power of two");
static_assert(std::is_standard_layout_v<PacketHeader>);
static_assert(BinarySerializer<PacketHeader>::width >= 8);

#if 0
static_assert(is_power_of_two(48), "intentional diagnostic example");
struct RuntimeOwned {
    RuntimeOwned(const RuntimeOwned&);
    virtual ~RuntimeOwned();
};
using RejectedSerializer = BinarySerializer<RuntimeOwned>;
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage07/section04/static_assert"};

    LEARN_EXPECT(checks, is_power_of_two(1));
    LEARN_EXPECT(checks, is_power_of_two(1'024));
    LEARN_EXPECT(checks, !is_power_of_two(0));
    LEARN_EXPECT(checks, !is_power_of_two(1'000));
    LEARN_EXPECT_EQ(checks, BinarySerializer<PacketHeader>::width, sizeof(PacketHeader));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section04/static_assert", run>;

}  // namespace

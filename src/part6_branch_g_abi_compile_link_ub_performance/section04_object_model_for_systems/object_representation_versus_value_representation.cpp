// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : object_representation_versus_value_representation
// Topic id : part6/g/section04/object_representation_versus_value_representation

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <bit>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace {

struct PossiblyPadded {
    char tag;
    int value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/object_representation_versus_value_representation"};

    static_assert(sizeof(std::uint32_t) * CHAR_BIT == 32);
    static_assert(std::numeric_limits<std::uint32_t>::digits == 32);
    static_assert(std::has_unique_object_representations_v<std::uint32_t>);
    constexpr std::uint32_t value = 0x01020304U;
    constexpr auto representation = std::bit_cast<std::array<std::byte, sizeof(value)>>(value);
    static_assert(std::bit_cast<std::uint32_t>(representation) == value);
    LEARN_EXPECT_EQ(checks, representation.size(), sizeof(value));

    // A class may contain padding: sizeof is object representation size, not
    // the sum of value-bearing members. Never compare such objects with memcmp.
    LEARN_EXPECT(checks, sizeof(PossiblyPadded) >= sizeof(char) + sizeof(int));
    const PossiblyPadded left{'x', 42};
    const PossiblyPadded right{'x', 42};
    LEARN_EXPECT(checks, left.tag == right.tag && left.value == right.value);

    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/g/section04/object_representation_versus_value_representation", run>;

}  // namespace

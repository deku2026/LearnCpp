// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : one_definition_rule_deep
// Topic id : part6/g/section02/one_definition_rule_deep
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/g/section02/one_definition_rule_deep";

inline constexpr int protocol_version = 4;

[[nodiscard]] inline constexpr int scale(int value) {
    return value * protocol_version;
}

struct PacketLayout {
    int id{};
    int payload_size{};
};

template <class T>
[[nodiscard]] constexpr T twice(T value) {
    return value + value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, scale(3), 12);
    LEARN_EXPECT_EQ(checks, twice(21), 42);
    static_assert(std::is_standard_layout_v<PacketLayout>);
    const PacketLayout packet{7, 42};
    LEARN_EXPECT_EQ(checks, packet.id, 7);
    LEARN_EXPECT_EQ(checks, packet.payload_size, 42);
    LEARN_EXPECT(checks, sizeof(PacketLayout) >= sizeof(int) * 2U);

#if 0
    // Defining a non-inline function in a header creates multiple definitions.
    int bad_header_function() { return 1; }
    // Token-different inline/class definitions across TUs violate the ODR and
    // may be ill-formed with no diagnostic required.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section02/one_definition_rule_deep", run>;

}  // namespace

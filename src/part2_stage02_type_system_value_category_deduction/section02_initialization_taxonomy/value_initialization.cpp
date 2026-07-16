// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : value_initialization
// Topic id : part2/stage02/section02/value_initialization
//

#include "learn/example_support.hpp"

#include <array>
#include <type_traits>

namespace {

struct Packet {
    int code;
    bool ready;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int scalar{};
    std::array<int, 3> values{};
    Packet packet{};
    static_assert(std::is_aggregate_v<Packet>);

    learn::ExampleChecks checks{"part2/stage02/section02/value_initialization"};
    LEARN_EXPECT_EQ(checks, scalar, 0);
    LEARN_EXPECT_EQ(checks, values[0] + values[1] + values[2], 0);
    LEARN_EXPECT_EQ(checks, packet.code, 0);
    LEARN_EXPECT(checks, !packet.ready);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/value_initialization", run>;

}  // namespace

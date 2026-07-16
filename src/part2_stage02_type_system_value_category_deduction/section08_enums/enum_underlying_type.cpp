// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : enum_underlying_type
// Topic id : part2/stage02/section08/enum_underlying_type
//

#include "learn/example_support.hpp"

#include <cstdint>
#include <type_traits>

namespace {

enum class WireState : std::uint8_t {
    idle = 1,
    busy = 200,
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    using Storage = std::underlying_type_t<WireState>;
    static_assert(std::is_same_v<Storage, std::uint8_t>);
    static_assert(sizeof(WireState) == sizeof(Storage));

    const auto encoded = static_cast<Storage>(WireState::busy);
    learn::ExampleChecks checks{"part2/stage02/section08/enum_underlying_type"};
    LEARN_EXPECT_EQ(checks, encoded, static_cast<Storage>(200));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/enum_underlying_type", run>;

}  // namespace

// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : to_underlying_cpp23
// Topic id : part2/stage02/section08/to_underlying_cpp23
//

#include "learn/example_support.hpp"

#include <cstdint>
#include <type_traits>
#include <utility>

namespace {

enum class Permission : std::uint16_t {
    read = 0x01,
    write = 0x02,
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
    static_assert(std::is_same_v<decltype(std::to_underlying(Permission::read)), std::uint16_t>);
    constexpr auto encoded = std::to_underlying(Permission::write);

    learn::ExampleChecks checks{"part2/stage02/section08/to_underlying_cpp23"};
    LEARN_EXPECT_EQ(checks, encoded, std::uint16_t{0x02});
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section08/to_underlying_cpp23", "__cpp_lib_to_underlying");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/to_underlying_cpp23", run>;

}  // namespace

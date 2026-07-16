// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : is_scoped_enum_cpp23
// Topic id : part2/stage02/section08/is_scoped_enum_cpp23
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

enum LegacyMode {
    legacy_mode,
};

enum class ScopedMode {
    scoped_mode,
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
    static_assert(!std::is_scoped_enum_v<LegacyMode>);
    static_assert(std::is_scoped_enum_v<ScopedMode>);
    static_assert(!std::is_scoped_enum_v<int>);

    learn::ExampleChecks checks{"part2/stage02/section08/is_scoped_enum_cpp23"};
    LEARN_EXPECT(checks, std::is_scoped_enum_v<ScopedMode>);
    LEARN_EXPECT(checks, !std::is_scoped_enum_v<LegacyMode>);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section08/is_scoped_enum_cpp23",
                                             "__cpp_lib_is_scoped_enum");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/is_scoped_enum_cpp23", run>;

}  // namespace

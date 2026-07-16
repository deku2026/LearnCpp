// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : designated_initializers_cpp20
// Topic id : part2/stage02/section09/designated_initializers_cpp20
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct Options {
    int retries = 3;
    bool verbose = false;
    int timeout_ms = 1000;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_designated_initializers) && __cpp_designated_initializers >= 201707L
    static_assert(std::is_aggregate_v<Options>);
    Options options{.retries = 5, .verbose = true};

    learn::ExampleChecks checks{"part2/stage02/section09/designated_initializers_cpp20"};
    LEARN_EXPECT_EQ(checks, options.retries, 5);
    LEARN_EXPECT(checks, options.verbose);
    LEARN_EXPECT_EQ(checks, options.timeout_ms, 1000);

    // C++ designators must follow declaration order and name direct members.
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section09/designated_initializers_cpp20",
                                             "__cpp_designated_initializers");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section09/designated_initializers_cpp20", run>;

}  // namespace

// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : using_enum_cpp20
// Topic id : part2/stage02/section08/using_enum_cpp20
//

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>

namespace {

enum class Status {
    ready,
    waiting,
    failed,
};

#if defined(__cpp_using_enum) && __cpp_using_enum >= 201907L
constexpr std::string_view describe(Status status) {
    using enum Status;
    switch (status) {
        case ready:
            return "ready";
        case waiting:
            return "waiting";
        case failed:
            return "failed";
    }
    return "unknown";
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_enum_v<Status>);

#if defined(__cpp_using_enum) && __cpp_using_enum >= 201907L
    learn::ExampleChecks checks{"part2/stage02/section08/using_enum_cpp20"};
    LEARN_EXPECT_EQ(checks, describe(Status::ready), std::string_view{"ready"});
    LEARN_EXPECT_EQ(checks, describe(Status::failed), std::string_view{"failed"});
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section08/using_enum_cpp20", "__cpp_using_enum");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/using_enum_cpp20", run>;

}  // namespace

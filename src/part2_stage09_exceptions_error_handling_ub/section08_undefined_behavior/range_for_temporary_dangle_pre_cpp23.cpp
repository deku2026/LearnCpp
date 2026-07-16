// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : range_for_temporary_dangle_pre_cpp23
// Topic id : part2/stage09/section08/range_for_temporary_dangle_pre_cpp23
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/range_for_temporary_dangle_pre_cpp23";

std::vector<std::string> make_words() {
    return {"safe", "owner"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::string observed;
    auto words = make_words();
    for (const char ch : words.front()) {  // Named owner is portable across language versions.
        observed.push_back(ch);
    }
    LEARN_EXPECT_EQ(checks, observed, std::string{"safe"});

#if defined(__cpp_range_based_for) && __cpp_range_based_for >= 202211L
    std::string cxx23;
    for (const char ch : make_words().front()) {  // C++23 extends relevant for-range temporaries.
        cxx23.push_back(ch);
    }
    LEARN_EXPECT_EQ(checks, cxx23, std::string{"safe"});
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage09/section08/range_for_temporary_dangle_pre_cpp23", run>;

}  // namespace

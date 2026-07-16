// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : ctest_integration
// Topic id : part4/section04/ctest_integration

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <string_view>

namespace {

struct CTestCase {
    std::string_view name;
    std::string_view label;
    int exit_code;
    int elapsed_ms;
    int timeout_ms;
    bool will_fail{};
};

[[nodiscard]] constexpr bool ctest_passes(const CTestCase& test) noexcept {
    if (test.elapsed_ms > test.timeout_ms) {
        return false;
    }
    const bool command_succeeded = test.exit_code == 0;
    return test.will_fail ? !command_succeeded : command_succeeded;
}

[[nodiscard]] constexpr int selected_count(std::string_view label) noexcept {
    constexpr std::array tests{
        CTestCase{"unit.parser", "unit", 0, 8, 500},
        CTestCase{"integration.store", "integration", 0, 60, 1'000},
        CTestCase{"rejects.invalid.cli", "unit", 2, 4, 500, true},
    };
    int count = 0;
    for (const auto& test : tests) {
        count += test.label == label ? 1 : 0;
    }
    return count;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/ctest_integration"};

    constexpr CTestCase normal{"unit.parser", "unit", 0, 8, 500};
    constexpr CTestCase expected_failure{"invalid.cli", "unit", 2, 4, 500, true};
    constexpr CTestCase timeout{"integration.network", "integration", 0, 1'100, 1'000};
    static_assert(ctest_passes(normal));
    static_assert(ctest_passes(expected_failure));
    static_assert(!ctest_passes(timeout));
    LEARN_EXPECT_EQ(checks, selected_count("unit"), 2);
    LEARN_EXPECT_EQ(checks, selected_count("integration"), 1);

    constexpr std::string_view registration =
        "enable_testing(); add_test(NAME unit.parser COMMAND app --self-test); "
        "set_tests_properties(unit.parser PROPERTIES LABELS unit TIMEOUT 10)";
    constexpr std::string_view ci_command = "ctest --test-dir build --output-on-failure -L unit";
    LEARN_EXPECT(checks, registration.contains("add_test"));
    LEARN_EXPECT(checks, registration.contains("TIMEOUT"));
    LEARN_EXPECT(checks, ci_command.contains("--output-on-failure"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/ctest_integration", run>;

}  // namespace

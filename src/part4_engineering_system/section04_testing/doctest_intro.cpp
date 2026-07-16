// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : doctest_intro
// Topic id : part4/section04/doctest_intro

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <string_view>

namespace {

struct TestCase {
    std::string_view name;
    bool (*body)();
};

[[nodiscard]] bool arithmetic_case() noexcept {
    return 6 * 7 == 42;
}

[[nodiscard]] bool string_case() noexcept {
    constexpr std::string_view value{"small test binary"};
    return value.starts_with("small") && value.ends_with("binary");
}

struct RunSummary {
    int selected{};
    int passed{};
};

[[nodiscard]] RunSummary run_cases(std::string_view filter) {
    constexpr std::array tests{TestCase{"arithmetic", arithmetic_case}, TestCase{"string", string_case}};
    RunSummary summary;
    for (const auto& test : tests) {
        if (!filter.empty() && !test.name.contains(filter)) {
            continue;
        }
        ++summary.selected;
        summary.passed += test.body() ? 1 : 0;
    }
    return summary;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/doctest_intro"};

    const auto all = run_cases("");
    LEARN_EXPECT_EQ(checks, all.selected, 2);
    LEARN_EXPECT_EQ(checks, all.passed, 2);

    const auto filtered = run_cases("string");
    LEARN_EXPECT_EQ(checks, filtered.selected, 1);
    LEARN_EXPECT_EQ(checks, filtered.passed, 1);

    // In a real doctest executable, define the implementation macro in one TU only.
    constexpr std::string_view standalone = "DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN";
    constexpr std::string_view embedded = "doctest::Context context; context.run()";
    LEARN_EXPECT(checks, standalone.contains("WITH_MAIN"));
    LEARN_EXPECT(checks, embedded.contains("Context"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/doctest_intro", run>;

}  // namespace

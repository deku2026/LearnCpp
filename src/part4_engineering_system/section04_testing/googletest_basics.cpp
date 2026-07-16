// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : googletest_basics
// Topic id : part4/section04/googletest_basics

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <exception>
#include <string_view>

namespace {

struct TestStats {
    int assertions{};
    int failures{};
};

class AbortCurrentTest final : public std::exception {};

class GTestSemantics {
public:
    explicit GTestSemantics(TestStats& stats) : stats_(stats) {}

    void expect(bool condition) noexcept {  // EXPECT_*: record and continue.
        ++stats_.assertions;
        stats_.failures += condition ? 0 : 1;
    }

    void require(bool condition) {  // ASSERT_*: record and abort only this case.
        ++stats_.assertions;
        if (!condition) {
            ++stats_.failures;
            throw AbortCurrentTest{};
        }
    }

private:
    TestStats& stats_;
};

template <class TestBody>
[[nodiscard]] TestStats run_test_case(TestBody body) {
    TestStats stats;
    GTestSemantics api{stats};
    try {
        body(api);
    } catch (const AbortCurrentTest&) {
    }
    return stats;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/googletest_basics"};

    const auto passing = run_test_case([](GTestSemantics& test) {
        test.require(2 + 2 == 4);
        test.expect(std::string_view{"cpp"}.size() == 3);
    });
    LEARN_EXPECT_EQ(checks, passing.assertions, 2);
    LEARN_EXPECT_EQ(checks, passing.failures, 0);

    bool reached_after_expect = false;
    bool reached_after_require = false;
    const auto illustrative_failure = run_test_case([&](GTestSemantics& test) {
        test.expect(false);
        reached_after_expect = true;
        test.require(false);
        reached_after_require = true;
    });
    LEARN_EXPECT_EQ(checks, illustrative_failure.failures, 2);
    LEARN_EXPECT(checks, reached_after_expect);
    LEARN_EXPECT(checks, !reached_after_require);

    constexpr std::string_view cmake =
        "target_link_libraries(unit_tests PRIVATE GTest::gtest_main); "
        "gtest_discover_tests(unit_tests)";
    LEARN_EXPECT(checks, cmake.contains("GTest::gtest_main"));
    LEARN_EXPECT(checks, cmake.contains("gtest_discover_tests"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/googletest_basics", run>;

}  // namespace

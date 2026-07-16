// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : nsdmi_default_member_init
// Topic id : part2/stage04/section01/nsdmi_default_member_init
// References: [class.mem.general], [class.base.init]

#include "learn/example_support.hpp"

#include <chrono>
#include <string>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section01/nsdmi_default_member_init";

struct RetryPolicy {
    int attempts = 3;
    std::chrono::milliseconds delay{100};
    std::string label = "default";

    RetryPolicy() = default;
    explicit RetryPolicy(int count) : attempts(count) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    RetryPolicy defaults;
    RetryPolicy overridden{5};

    LEARN_EXPECT_EQ(checks, defaults.attempts, 3);
    LEARN_EXPECT_EQ(checks, defaults.delay, std::chrono::milliseconds{100});
    LEARN_EXPECT_EQ(checks, overridden.attempts, 5);
    LEARN_EXPECT_EQ(checks, overridden.label, std::string{"default"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section01/nsdmi_default_member_init", run>;

}  // namespace

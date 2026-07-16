// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : static_member
// Topic id : part2/stage04/section06/static_member
// References: [class.static.data], [class.static.mfct]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section06/static_member";

class InstanceCounter {
public:
    InstanceCounter() { ++alive_; }
    ~InstanceCounter() { --alive_; }
    InstanceCounter(const InstanceCounter&) = delete;
    InstanceCounter& operator=(const InstanceCounter&) = delete;

    [[nodiscard]] static int alive() noexcept { return alive_; }

private:
    inline static int alive_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, InstanceCounter::alive(), 0);
    {
        InstanceCounter first;
        LEARN_EXPECT_EQ(checks, InstanceCounter::alive(), 1);
        {
            InstanceCounter second;
            LEARN_EXPECT_EQ(checks, InstanceCounter::alive(), 2);
        }
        LEARN_EXPECT_EQ(checks, InstanceCounter::alive(), 1);
    }
    LEARN_EXPECT_EQ(checks, InstanceCounter::alive(), 0);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section06/static_member", run>;

}  // namespace

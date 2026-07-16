// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section02_raii
// Item     : raii_exception_safety_pairing
// Topic id : part2/stage04/section02/raii_exception_safety_pairing
// References: [stmt.dcl], [except.handle], C++ Core Guidelines E.6

#include "learn/example_support.hpp"

#include <stdexcept>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section02/raii_exception_safety_pairing";

class TrackedResource {
public:
    TrackedResource() { ++live_; }
    ~TrackedResource() { --live_; }
    TrackedResource(const TrackedResource&) = delete;
    TrackedResource& operator=(const TrackedResource&) = delete;

    [[nodiscard]] static int live() noexcept { return live_; }

private:
    inline static int live_{};
};

void operation_that_fails() {
    TrackedResource first;
    TrackedResource second;
    if (TrackedResource::live() == 2) {
        throw std::runtime_error{"simulated failure"};
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, TrackedResource::live(), 0);
    checks.expect_throws<std::runtime_error>(operation_that_fails, "operation_that_fails()");
    LEARN_EXPECT_EQ(checks, TrackedResource::live(), 0);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section02/raii_exception_safety_pairing", run>;

}  // namespace

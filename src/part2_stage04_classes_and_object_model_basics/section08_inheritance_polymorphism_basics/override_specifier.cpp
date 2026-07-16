// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : override_specifier
// Topic id : part2/stage04/section08/override_specifier
// References: [class.virtual]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section08/override_specifier";

struct Device {
    virtual ~Device() = default;
    [[nodiscard]] virtual int status() const noexcept { return 0; }
};

struct Sensor : Device {
    [[nodiscard]] int status() const noexcept override { return 200; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const Sensor sensor;
    const Device& device = sensor;
    LEARN_EXPECT_EQ(checks, device.status(), 200);
    // A mismatched signature with `override` would be diagnosed at compile time.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section08/override_specifier", run>;

}  // namespace

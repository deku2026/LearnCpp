// A CRTP mixin injects reusable behavior that depends on the derived interface.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_inject_behavior
// Topic id : part2/stage06/section06/crtp_inject_behavior
//
// RecordText supplies formatting while Sensor supplies only label() and value().

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

template <class Derived>
struct RecordText {
    [[nodiscard]] std::string as_record() const {
        const auto& self = static_cast<const Derived&>(*this);
        return self.label() + "=" + std::to_string(self.value());
    }
};

struct Sensor : RecordText<Sensor> {
    std::string name;
    int reading{};

    Sensor(std::string sensor_name, int sensor_reading) : name(std::move(sensor_name)), reading(sensor_reading) {}

    [[nodiscard]] const std::string& label() const { return name; }
    [[nodiscard]] int value() const { return reading; }
};

static_assert(std::is_base_of_v<RecordText<Sensor>, Sensor>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section06/crtp_inject_behavior"};
    const Sensor sensor{"temperature", 23};
    LEARN_EXPECT_EQ(checks, sensor.as_record(), std::string{"temperature=23"});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_inject_behavior", run>;

}  // namespace

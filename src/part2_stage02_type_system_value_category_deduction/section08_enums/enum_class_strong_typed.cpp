// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : enum_class_strong_typed
// Topic id : part2/stage02/section08/enum_class_strong_typed
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

enum class TrafficLight {
    red,
    amber,
    green,
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_enum_v<TrafficLight>);
    static_assert(!std::is_convertible_v<TrafficLight, int>);

    TrafficLight light = TrafficLight::red;
    light = TrafficLight::green;

    learn::ExampleChecks checks{"part2/stage02/section08/enum_class_strong_typed"};
    LEARN_EXPECT(checks, light == TrafficLight::green);
    LEARN_EXPECT(checks, light != TrafficLight::amber);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/enum_class_strong_typed", run>;

}  // namespace

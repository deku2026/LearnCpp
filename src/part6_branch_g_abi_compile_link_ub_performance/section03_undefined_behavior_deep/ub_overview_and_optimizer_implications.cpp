// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : ub_overview_and_optimizer_implications
// Topic id : part6/g/section03/ub_overview_and_optimizer_implications

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <limits>
#include <optional>
#include <string_view>

namespace {

enum class BehaviorClass { defined, implementation_defined, unspecified, undefined };

struct BehaviorExample {
    std::string_view description;
    BehaviorClass category;
};

[[nodiscard]] constexpr std::optional<int> checked_increment(int value) noexcept {
    if (value == std::numeric_limits<int>::max()) {
        return std::nullopt;
    }
    return value + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/ub_overview_and_optimizer_implications"};

    constexpr std::array catalog{
        BehaviorExample{"unsigned integer wrap", BehaviorClass::defined},
        BehaviorExample{"size of a pointer", BehaviorClass::implementation_defined},
        BehaviorExample{"function argument evaluation order", BehaviorClass::unspecified},
        BehaviorExample{"signed integer overflow", BehaviorClass::undefined},
    };
    static_assert(catalog[0].category == BehaviorClass::defined);
    static_assert(catalog[3].category == BehaviorClass::undefined);
    LEARN_EXPECT_EQ(checks, checked_increment(41), std::optional<int>{42});
    LEARN_EXPECT(checks, !checked_increment(std::numeric_limits<int>::max()).has_value());

#if 0
    // Once signed overflow is reachable, the optimizer may assume that path
    // never occurs and transform surrounding control flow accordingly.
    int bad_increment(int value) { return value + 1; }
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/ub_overview_and_optimizer_implications", run>;

}  // namespace

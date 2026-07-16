// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : link_time_optimization
// Topic id : part6/g/section02/link_time_optimization
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <array>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/g/section02/link_time_optimization";

enum class OptimizationScope { one_translation_unit, whole_program };

struct BuildMode {
    std::string_view name;
    OptimizationScope scope;
    bool exposes_intermediate_representation_across_tus;
    bool guarantees_a_specific_transformation;
};

constexpr std::array modes{
    BuildMode{"ordinary optimized build", OptimizationScope::one_translation_unit, false, false},
    BuildMode{"link-time optimized build", OptimizationScope::whole_program, true, false},
};

[[nodiscard]] constexpr int library_step(int value) {
    return value * 2;
}
[[nodiscard]] constexpr int application_step(int value) {
    return library_step(value) + 2;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, application_step(20), 42);
    LEARN_EXPECT(checks, !modes[0].exposes_intermediate_representation_across_tus);
    LEARN_EXPECT(checks, modes[1].exposes_intermediate_representation_across_tus);
    LEARN_EXPECT(checks, !modes[1].guarantees_a_specific_transformation);

    // LTO permits cross-TU analysis; it does not guarantee inlining or removal.
    // Linker section garbage collection may remove code even without LTO.
    // Verify benefits and costs on actual Release artifacts.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section02/link_time_optimization", run>;

}  // namespace

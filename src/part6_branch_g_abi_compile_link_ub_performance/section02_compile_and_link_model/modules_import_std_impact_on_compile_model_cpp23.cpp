// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : modules_import_std_impact_on_compile_model_cpp23
// Topic id : part6/g/section02/modules_import_std_impact_on_compile_model_cpp23
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <array>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/g/section02/modules_import_std_impact_on_compile_model_cpp23";

struct CompileModel {
    std::string_view description;
    int repeated_library_parses_per_consumer;
    bool requires_prebuilt_interface_artifact;
    bool sensitive_to_interface_build_flags;
};

constexpr std::array models{
    CompileModel{"textual standard-library headers", 1, false, false},
    CompileModel{"prebuilt standard-library interface", 0, true, true},
};

[[nodiscard]] constexpr int aggregate_parse_work(const CompileModel& model, int consumers) {
    return model.repeated_library_parses_per_consumer * consumers;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr int consumers = 8;
    LEARN_EXPECT_EQ(checks, aggregate_parse_work(models[0], consumers), 8);
    LEARN_EXPECT_EQ(checks, aggregate_parse_work(models[1], consumers), 0);
    LEARN_EXPECT(checks, models[1].requires_prebuilt_interface_artifact);
    LEARN_EXPECT(checks, models[1].sensitive_to_interface_build_flags);

    // This repository intentionally keeps examples header-based: current
    // standard-library interface packaging and toolchain workflows are still
    // implementation-specific. The model records trade-offs without using
    // experimental module syntax.
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part6/g/section02/modules_import_std_impact_on_compile_model_cpp23", run>;

}  // namespace

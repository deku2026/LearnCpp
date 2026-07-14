// LearnCpp placeholder
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : compile_time_type_calculation
// Topic id : part2/stage06/section07/compile_time_type_calculation
//
// TODO: read cppreference, sketch a minimal example, check godbolt / C++ Insights,
//       then replace this empty run() body with real demo code.

#include "learn/topic_registry.hpp"

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/compile_time_type_calculation", run>;

}  // namespace

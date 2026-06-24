// LearnCpp placeholder
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : ctad_class_template_argument_deduction_cpp17
// Topic id : part2/stage06/section03/ctad_class_template_argument_deduction_cpp17
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage06/section03/ctad_class_template_argument_deduction_cpp17", run>;

}  // namespace

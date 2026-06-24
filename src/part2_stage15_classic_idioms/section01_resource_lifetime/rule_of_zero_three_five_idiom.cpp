// LearnCpp placeholder
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : rule_of_zero_three_five_idiom
// Topic id : part2/stage15/section01/rule_of_zero_three_five_idiom
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/rule_of_zero_three_five_idiom", run>;

}  // namespace

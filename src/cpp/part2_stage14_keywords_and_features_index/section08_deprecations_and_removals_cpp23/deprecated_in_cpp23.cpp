// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : deprecated_in_cpp23
// Topic id : part2/stage14/section08/deprecated_in_cpp23
//
// Covers: items deprecated in C++23 (safe notes)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

void demo_basics() {
    // Prefer modern replacements over deprecated facilities.
    std::string s = "use string not codecvt for new code";
    assert(!s.empty());
}

void demo_intermediate() {
    // std::aligned_storage / aligned_union are deprecated — use alignas + array.
    alignas(16) unsigned char buf[16]{};
    assert(buf[0] == 0);
}

void demo_expert() {
    // Keep builds clean: avoid deprecated headers/APIs in new topics.
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section08/deprecated_in_cpp23", run>;

}  // namespace

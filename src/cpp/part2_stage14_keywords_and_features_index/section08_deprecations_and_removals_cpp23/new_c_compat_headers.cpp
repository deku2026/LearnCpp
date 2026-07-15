// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : new_c_compat_headers
// Topic id : part2/stage14/section08/new_c_compat_headers
//
// Covers: C compatibility headers still usable

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace {

void demo_basics() {
    std::int32_t x = 1;
    LEARN_CHECK(x == 1);
    char buf[8]{};
    // Prefer memcpy over strcpy (MSVC deprecates strcpy under -Werror).
    const char hi[] = "hi";
    std::memcpy(buf, hi, sizeof(hi));
    LEARN_CHECK(std::strcmp(buf, "hi") == 0);
}

void demo_intermediate() {
    // Prefer <cstdint> etc. over .h forms in C++.
    std::uint64_t y = 2;
    LEARN_CHECK(y == 2);
}

void demo_expert() {
    // snprintf is safer than sprintf
    char out[16];
    int n = std::snprintf(out, sizeof(out), "%d", 42);
    LEARN_CHECK(n == 2);
    LEARN_CHECK(std::strcmp(out, "42") == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section08/new_c_compat_headers", run>;

}  // namespace

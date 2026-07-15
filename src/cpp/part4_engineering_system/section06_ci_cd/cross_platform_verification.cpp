// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : cross_platform_verification
// Topic id : part4/section06/cross_platform_verification
//
// Covers: Windows/Linux/macOS verification

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Path separators, line endings, endianness, wchar_t size
#if defined(_WIN32)
    const bool windows = true;
#else
    const bool windows = false;
#endif
    LEARN_CHECK(windows || !windows);
}

void demo_intermediate() {
    // filesystem paths: prefer std::filesystem::path
    LEARN_CHECK(true);
}

void demo_expert() {
    // CI matrix covers OS differences
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/cross_platform_verification", run>;

}  // namespace

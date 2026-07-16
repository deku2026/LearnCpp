// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : sanitizer_compatibility_matrix
// Topic id : part4/section05/sanitizer_compatibility_matrix

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <string_view>

namespace {

enum class Sanitizer { address, undefined_behavior, thread, memory, leak };

struct SanitizerInfo {
    Sanitizer tool;
    std::string_view detects;
    bool requires_instrumented_dependencies;
};

constexpr std::array sanitizer_info{
    SanitizerInfo{Sanitizer::address, "out-of-bounds and use-after-free", false},
    SanitizerInfo{Sanitizer::undefined_behavior, "language undefined behavior", false},
    SanitizerInfo{Sanitizer::thread, "data races", false},
    SanitizerInfo{Sanitizer::memory, "uninitialized reads", true},
    SanitizerInfo{Sanitizer::leak, "unreachable allocations", false},
};

// A deliberately conservative CI policy; supported combinations still vary by platform.
[[nodiscard]] constexpr bool same_lane(Sanitizer left, Sanitizer right) noexcept {
    if (left == right) {
        return true;
    }
    const bool address_and_undefined = (left == Sanitizer::address && right == Sanitizer::undefined_behavior) ||
                                       (right == Sanitizer::address && left == Sanitizer::undefined_behavior);
    const bool address_and_leak = (left == Sanitizer::address && right == Sanitizer::leak) ||
                                  (right == Sanitizer::address && left == Sanitizer::leak);
    return address_and_undefined || address_and_leak;
}

[[nodiscard]] constexpr const SanitizerInfo& describe(Sanitizer tool) noexcept {
    for (const auto& info : sanitizer_info) {
        if (info.tool == tool) {
            return info;
        }
    }
    return sanitizer_info.front();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/sanitizer_compatibility_matrix"};

    static_assert(same_lane(Sanitizer::address, Sanitizer::undefined_behavior));
    static_assert(same_lane(Sanitizer::address, Sanitizer::leak));
    static_assert(!same_lane(Sanitizer::address, Sanitizer::thread));
    static_assert(!same_lane(Sanitizer::thread, Sanitizer::memory));
    LEARN_EXPECT(checks, describe(Sanitizer::memory).requires_instrumented_dependencies);
    LEARN_EXPECT(checks, describe(Sanitizer::thread).detects.contains("races"));

    constexpr std::array lanes{
        std::string_view{"ASan + UBSan (Linux/Clang or GCC)"},
        std::string_view{"TSan alone"},
        std::string_view{"MSan alone with an instrumented dependency stack"},
    };
    LEARN_EXPECT_EQ(checks, lanes.size(), 3U);
    LEARN_EXPECT(checks, lanes[1].contains("alone"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/sanitizer_compatibility_matrix", run>;

}  // namespace

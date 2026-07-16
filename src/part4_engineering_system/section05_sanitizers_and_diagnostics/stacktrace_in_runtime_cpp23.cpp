// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : stacktrace_in_runtime_cpp23
// Topic id : part4/section05/stacktrace_in_runtime_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <string_view>

#if defined(__has_include)
#if __has_include(<stacktrace>)
#include <stacktrace>
#endif
#if __has_include(<stacktrace>) && defined(__cpp_lib_stacktrace) && \
    __cpp_lib_stacktrace >= 202011L &&                              \
    (!defined(LEARNCPP_HAS_LINKABLE_STACKTRACE) || LEARNCPP_HAS_LINKABLE_STACKTRACE)
#define LEARNCPP_HAS_STACKTRACE 1
#else
#define LEARNCPP_HAS_STACKTRACE 0
#endif
#else
#define LEARNCPP_HAS_STACKTRACE 0
#endif

namespace {

#if LEARNCPP_HAS_STACKTRACE
[[nodiscard]] std::size_t capture_bounded_depth() {
    constexpr std::size_t maximum_depth = 16;
    return std::stacktrace::current(0, maximum_depth).size();
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/stacktrace_in_runtime_cpp23"};

#if LEARNCPP_HAS_STACKTRACE
    const auto depth = capture_bounded_depth();
    LEARN_EXPECT(checks, depth <= 16U);
    constexpr long feature = __cpp_lib_stacktrace;
    static_assert(feature >= 202011L);
#else
    LEARN_EXPECT_EQ(checks, LEARNCPP_HAS_STACKTRACE, 0);
#endif

    constexpr std::string_view guidance =
        "gate on __has_include(<stacktrace>) and __cpp_lib_stacktrace; keep symbols and debug info";
    LEARN_EXPECT(checks, guidance.contains("__cpp_lib_stacktrace"));
    LEARN_EXPECT(checks, guidance.contains("debug info"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/stacktrace_in_runtime_cpp23", run>;

}  // namespace

#undef LEARNCPP_HAS_STACKTRACE

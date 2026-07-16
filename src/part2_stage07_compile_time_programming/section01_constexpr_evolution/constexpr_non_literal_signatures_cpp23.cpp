// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_non_literal_signatures_cpp23
// Topic id : part2/stage07/section01/constexpr_non_literal_signatures_cpp23
// References: N4950 [dcl.constexpr], [basic.types]; P2448R2.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202207L && \
    (!defined(LEARNCPP_HAS_CONSTEXPR_NON_LITERAL_SIGNATURES) || LEARNCPP_HAS_CONSTEXPR_NON_LITERAL_SIGNATURES)

struct RuntimeOnly {
    int value;
    ~RuntimeOnly() noexcept {}  // A non-constexpr destructor makes this non-literal.
};

// C++23 no longer requires constexpr function parameter and return types to
// be literal types. These functions remain useful on runtime-only objects.
constexpr RuntimeOnly make_runtime_only(int value) noexcept {
    return RuntimeOnly{value};
}

constexpr int inspect_runtime_only(const RuntimeOnly& object) noexcept {
    return object.value;
}

static_assert(!std::is_trivially_destructible_v<RuntimeOnly>);
static_assert(std::is_same_v<decltype(make_runtime_only(1)), RuntimeOnly>);

#if 0
// The relaxed signature does not make RuntimeOnly usable as a constexpr object.
constexpr RuntimeOnly rejected = make_runtime_only(42);
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202207L && \
    (!defined(LEARNCPP_HAS_CONSTEXPR_NON_LITERAL_SIGNATURES) || LEARNCPP_HAS_CONSTEXPR_NON_LITERAL_SIGNATURES)
    ::learn::ExampleChecks checks{"part2/stage07/section01/constexpr_non_literal_signatures_cpp23"};

    auto object = make_runtime_only(42);
    LEARN_EXPECT_EQ(checks, object.value, 42);
    LEARN_EXPECT_EQ(checks, inspect_runtime_only(object), 42);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section01/constexpr_non_literal_signatures_cpp23",
                                               "__cpp_constexpr >= 202207L (P2448)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_non_literal_signatures_cpp23", run>;

}  // namespace

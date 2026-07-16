// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : attribute_visibility
// Topic id : part6/g/section01/attribute_visibility
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <string_view>

#if !defined(_WIN32) && defined(__has_attribute)
#if __has_attribute(visibility)
#define LEARN_PUBLIC_VISIBILITY __attribute__((visibility("default")))
constexpr bool kVisibilityAttributeAvailable = true;
#else
#define LEARN_PUBLIC_VISIBILITY
constexpr bool kVisibilityAttributeAvailable = false;
#endif
#else
#define LEARN_PUBLIC_VISIBILITY
constexpr bool kVisibilityAttributeAvailable = false;
#endif

// Visibility attributes affect only entities that already have external
// linkage; they cannot turn an anonymous-namespace function into an export.
LEARN_PUBLIC_VISIBILITY [[nodiscard]] int branch_g_public_entry(int value) noexcept {
    return value + 1;
}

namespace {

constexpr std::string_view kTopic = "part6/g/section01/attribute_visibility";

[[nodiscard]] int implementation_detail(int value) noexcept {
    return value * 2;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, branch_g_public_entry(4), 5);
    LEARN_EXPECT_EQ(checks, implementation_detail(4), 8);

#if !defined(_WIN32) && defined(__has_attribute)
#if __has_attribute(visibility)
    LEARN_EXPECT(checks, kVisibilityAttributeAvailable);
#else
    LEARN_EXPECT(checks, !kVisibilityAttributeAvailable);
#endif
#else
    LEARN_EXPECT(checks, !kVisibilityAttributeAvailable);
#endif
    // Pair an explicit default-visibility macro with the build option that
    // hides symbols by default. The spelling is intentionally platform-gated.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section01/attribute_visibility", run>;

}  // namespace

#undef LEARN_PUBLIC_VISIBILITY

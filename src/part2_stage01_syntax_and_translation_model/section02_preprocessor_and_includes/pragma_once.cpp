// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : pragma_once
// Topic id : part2/stage01/section02/pragma_once
// References: #pragma is implementation-defined in N4950 [cpp.pragma];
//             #pragma once is a widely implemented, non-standard extension.

#include "learn/example_support.hpp"

#include <array>

// This is the exact header spelling. It is opt-in because putting it in a .cpp
// file may itself produce an implementation diagnostic.
#if defined(LEARNCPP_STAGE01_EMIT_PRAGMA_ONCE)
#pragma once
#endif

namespace {

enum class ProtectionTechnique { macro_guard, pragma_once };

struct ProtectionProperties {
    ProtectionTechnique technique;
    bool uses_only_standard_cpp_directives;
    bool relies_on_implementation_file_identity;
};

inline constexpr std::array techniques{
    ProtectionProperties{ProtectionTechnique::macro_guard, true, false},
    ProtectionProperties{ProtectionTechnique::pragma_once, false, true},
};

static_assert(techniques[0].uses_only_standard_cpp_directives);
static_assert(!techniques[1].uses_only_standard_cpp_directives);
static_assert(techniques[1].relies_on_implementation_file_identity);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section02/pragma_once"};
    LEARN_EXPECT_EQ(checks, techniques[0].technique, ProtectionTechnique::macro_guard);
    LEARN_EXPECT(checks, techniques[0].uses_only_standard_cpp_directives);
    LEARN_EXPECT_EQ(checks, techniques[1].technique, ProtectionTechnique::pragma_once);
    LEARN_EXPECT(checks, !techniques[1].uses_only_standard_cpp_directives);
    LEARN_EXPECT(checks, techniques[1].relies_on_implementation_file_identity);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/pragma_once", run>;

}  // namespace

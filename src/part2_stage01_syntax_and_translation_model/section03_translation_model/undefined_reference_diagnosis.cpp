// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : undefined_reference_diagnosis
// Topic id : part2/stage01/section03/undefined_reference_diagnosis
// References: N4950 [basic.def.odr], [basic.link]. Diagnostic wording and
//             symbol encoding are implementation-specific.

#include "learn/example_support.hpp"

#include <array>
#include <type_traits>

namespace {

struct ExternalApi {
    static int declared_only(int);  // Well-formed declaration; intentionally no definition.
};

static_assert(requires { ExternalApi::declared_only(1); });
static_assert(std::is_same_v<decltype(ExternalApi::declared_only(1)), int>);

// Evaluating this call would create a reference for which the linker cannot
// find a matching definition, so the negative example is kept out of the build.
#if 0
int would_fail_to_link() {
    return ExternalApi::declared_only(21);
}
#endif

constexpr int linked_definition(int value) noexcept {
    return value * 2;
}

enum class LinkRequirement { declaration, defining_object, matching_signature, required_library };

struct LinkInputs {
    bool declaration_visible;
    bool defining_object_linked;
    bool signature_matches;
    bool required_library_linked;
};

constexpr bool resolves(const LinkInputs& inputs) noexcept {
    return inputs.declaration_visible && inputs.defining_object_linked && inputs.signature_matches &&
           inputs.required_library_linked;
}

inline constexpr LinkInputs healthy_link{true, true, true, true};
inline constexpr LinkInputs missing_definition{true, false, true, true};
inline constexpr LinkInputs signature_mismatch{true, true, false, true};
inline constexpr LinkInputs missing_library{true, true, true, false};

inline constexpr std::array common_causes{
    LinkRequirement::defining_object,
    LinkRequirement::matching_signature,
    LinkRequirement::required_library,
};

static_assert(resolves(healthy_link));
static_assert(!resolves(missing_definition));
static_assert(!resolves(signature_mismatch));
static_assert(!resolves(missing_library));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section03/undefined_reference_diagnosis"};

    LEARN_EXPECT_EQ(checks, linked_definition(21), 42);
    LEARN_EXPECT(checks, resolves(healthy_link));
    LEARN_EXPECT(checks, !resolves(missing_definition));
    LEARN_EXPECT(checks, !resolves(signature_mismatch));
    LEARN_EXPECT(checks, !resolves(missing_library));
    LEARN_EXPECT_EQ(checks, common_causes.size(), 3uz);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/undefined_reference_diagnosis", run>;

}  // namespace

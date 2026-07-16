// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : static_library_versus_dynamic_library
// Topic id : part6/g/section02/static_library_versus_dynamic_library
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <array>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/g/section02/static_library_versus_dynamic_library";

enum class LibraryKind { static_library, dynamic_library };

struct Tradeoff {
    LibraryKind kind;
    bool code_copied_into_executable;
    bool replace_without_relink_when_abi_compatible;
    bool suitable_for_runtime_plugins;
};

constexpr std::array choices{
    Tradeoff{LibraryKind::static_library, true, false, false},
    Tradeoff{LibraryKind::dynamic_library, false, true, true},
};

[[nodiscard]] constexpr const Tradeoff& describe(LibraryKind kind) {
    return choices[kind == LibraryKind::static_library ? 0U : 1U];
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const auto& static_choice = describe(LibraryKind::static_library);
    const auto& dynamic_choice = describe(LibraryKind::dynamic_library);
    LEARN_EXPECT(checks, static_choice.code_copied_into_executable);
    LEARN_EXPECT(checks, !static_choice.replace_without_relink_when_abi_compatible);
    LEARN_EXPECT(checks, dynamic_choice.replace_without_relink_when_abi_compatible);
    LEARN_EXPECT(checks, dynamic_choice.suitable_for_runtime_plugins);

    // Dynamic replacement additionally requires a compatible ABI, exported
    // symbols, loader identity/version policy, and compatible dependencies.
    // Actual size, sharing, startup, and optimization costs are measurements.
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part6/g/section02/static_library_versus_dynamic_library", run>;

}  // namespace

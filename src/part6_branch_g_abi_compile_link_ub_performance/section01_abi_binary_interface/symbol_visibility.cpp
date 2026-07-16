// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : symbol_visibility
// Topic id : part6/g/section01/symbol_visibility
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <array>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/g/section01/symbol_visibility";

enum class Visibility { exported, hidden };

struct SymbolPolicy {
    std::string_view name;
    Visibility visibility;
};

constexpr std::array public_surface{
    SymbolPolicy{"library_create", Visibility::exported},
    SymbolPolicy{"library_destroy", Visibility::exported},
    SymbolPolicy{"parse_internal_cache", Visibility::hidden},
    SymbolPolicy{"detail_allocator", Visibility::hidden},
};

[[nodiscard]] constexpr std::size_t exported_count() {
    std::size_t count{};
    for (const auto& symbol : public_surface) {
        if (symbol.visibility == Visibility::exported) {
            ++count;
        }
    }
    return count;
}

static_assert(exported_count() == 2);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, exported_count(), 2U);
    LEARN_EXPECT(checks, public_surface[0].visibility == Visibility::exported);
    LEARN_EXPECT(checks, public_surface[3].visibility == Visibility::hidden);

    // Visibility is a linker/platform property, not a standard C++ property.
    // A small explicit export surface reduces accidental ABI commitments.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section01/symbol_visibility", run>;

}  // namespace

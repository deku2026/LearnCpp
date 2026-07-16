// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : symbol_stripping
// Topic id : part6/g/section02/symbol_stripping
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <array>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/g/section02/symbol_stripping";

struct BinarySymbol {
    std::string_view name;
    bool dynamically_exported;
    bool needed_by_unresolved_dynamic_relocation;
    bool code_or_data_is_reachable;
};

[[nodiscard]] constexpr bool symbol_table_entry_must_remain(const BinarySymbol& symbol) {
    return symbol.dynamically_exported || symbol.needed_by_unresolved_dynamic_relocation;
}

constexpr std::array symbols{
    BinarySymbol{"plugin_create", true, true, true},
    BinarySymbol{"internal_helper", false, false, true},
    BinarySymbol{"local_debug_name", false, false, false},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT(checks, symbol_table_entry_must_remain(symbols[0]));
    LEARN_EXPECT(checks, !symbol_table_entry_must_remain(symbols[1]));
    LEARN_EXPECT(checks, symbols[1].code_or_data_is_reachable);
    LEARN_EXPECT(checks, !symbol_table_entry_must_remain(symbols[2]));

    // A local symbol name may be stripped even when its code remains reachable:
    // static relocations have already been resolved. Dynamic exports and names
    // needed by unresolved dynamic relocations are different. Keep a separate
    // symbol file when post-mortem diagnostics matter.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section02/symbol_stripping", run>;

}  // namespace

// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : symbol_resolution
// Topic id : part6/g/section02/symbol_resolution
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <array>
#include <optional>
#include <span>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/g/section02/symbol_resolution";

enum class SymbolState { declaration, definition };

struct Symbol {
    std::string_view name;
    SymbolState state;
    int object_file{};
};

[[nodiscard]] constexpr std::optional<int> resolve(std::span<const Symbol> symbols, std::string_view requested) {
    std::optional<int> provider;
    for (const auto& symbol : symbols) {
        if (symbol.name != requested || symbol.state != SymbolState::definition) {
            continue;
        }
        if (provider.has_value()) {
            return std::nullopt;  // multiple strong definitions
        }
        provider = symbol.object_file;
    }
    return provider;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr std::array symbols{
        Symbol{"parse", SymbolState::declaration, 1},
        Symbol{"parse", SymbolState::definition, 2},
        Symbol{"emit", SymbolState::definition, 3},
    };
    const auto parse_provider = resolve(symbols, "parse");
    LEARN_EXPECT(checks, parse_provider.has_value());
    LEARN_EXPECT_EQ(checks, *parse_provider, 2);
    LEARN_EXPECT(checks, !resolve(symbols, "missing").has_value());

    constexpr std::array duplicates{
        Symbol{"parse", SymbolState::definition, 2},
        Symbol{"parse", SymbolState::definition, 4},
    };
    LEARN_EXPECT(checks, !resolve(duplicates, "parse").has_value());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section02/symbol_resolution", run>;

}  // namespace

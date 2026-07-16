// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : per_cpp_object_file_independence
// Topic id : part2/stage01/section03/per_cpp_object_file_independence
// References: N4950 [lex.phases], [basic.link]. Object-file details are
//             implementation/toolchain concepts rather than C++ syntax.

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>

namespace {

enum class Symbol { program_entry, add };
enum class SymbolState { definition, unresolved_reference };

struct ObjectSymbol {
    Symbol symbol;
    SymbolState state;
};

inline constexpr std::array primary_object{
    ObjectSymbol{Symbol::program_entry, SymbolState::definition},
    ObjectSymbol{Symbol::add, SymbolState::unresolved_reference},
};

inline constexpr std::array arithmetic_object{
    ObjectSymbol{Symbol::add, SymbolState::definition},
};

template <std::size_t Size>
constexpr bool contains(const std::array<ObjectSymbol, Size>& object, Symbol symbol, SymbolState state) noexcept {
    for (const auto& entry : object) {
        if (entry.symbol == symbol && entry.state == state) {
            return true;
        }
    }
    return false;
}

constexpr bool link_resolves_add() noexcept {
    return contains(primary_object, Symbol::add, SymbolState::unresolved_reference) &&
           contains(arithmetic_object, Symbol::add, SymbolState::definition);
}

static_assert(contains(primary_object, Symbol::program_entry, SymbolState::definition));
static_assert(!contains(primary_object, Symbol::add, SymbolState::definition));
static_assert(contains(arithmetic_object, Symbol::add, SymbolState::definition));
static_assert(link_resolves_add());

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section03/per_cpp_object_file_independence"};

    LEARN_EXPECT(checks, contains(primary_object, Symbol::add, SymbolState::unresolved_reference));
    LEARN_EXPECT(checks, !contains(primary_object, Symbol::add, SymbolState::definition));
    LEARN_EXPECT(checks, contains(arithmetic_object, Symbol::add, SymbolState::definition));
    LEARN_EXPECT(checks, link_resolves_add());

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/per_cpp_object_file_independence", run>;

}  // namespace

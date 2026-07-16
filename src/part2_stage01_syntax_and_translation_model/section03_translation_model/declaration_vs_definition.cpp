// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : declaration_vs_definition
// Topic id : part2/stage01/section03/declaration_vs_definition
// References: N4950 [basic.def], [basic.def.odr], [dcl.dcl].

#include "learn/example_support.hpp"

#include <concepts>
#include <type_traits>

namespace {

template <class Type>
concept CompleteType = requires { sizeof(Type); };

struct ForwardDeclared;  // Declares a type name; the type is still incomplete here.

struct DefinedRecord {  // Declares and defines a complete class type.
    int value;
};

constexpr int doubled(int value) noexcept;  // Function declaration only.
extern const int shared_value;              // Variable declaration only.

constexpr int doubled(int value) noexcept {  // Function definition.
    return value * 2;
}

const int shared_value = 21;  // Variable definition; storage is provided here.

static_assert(!CompleteType<ForwardDeclared>);
static_assert(CompleteType<DefinedRecord>);
static_assert(std::is_same_v<decltype(doubled), int(int) noexcept>);
static_assert(doubled(shared_value) == 42);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section03/declaration_vs_definition"};

    const DefinedRecord record{shared_value};
    const int* shared_address = &shared_value;
    LEARN_EXPECT_EQ(checks, record.value, 21);
    LEARN_EXPECT_EQ(checks, doubled(record.value), 42);
    LEARN_EXPECT_EQ(checks, *shared_address, 21);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/declaration_vs_definition", run>;

}  // namespace

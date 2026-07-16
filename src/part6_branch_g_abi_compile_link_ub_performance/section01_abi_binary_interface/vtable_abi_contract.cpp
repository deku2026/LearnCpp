// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : vtable_abi_contract
// Topic id : part6/g/section01/vtable_abi_contract
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <memory>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/g/section01/vtable_abi_contract";

class Operation {
public:
    virtual ~Operation() = default;
    [[nodiscard]] virtual int apply(int value) const noexcept = 0;
    [[nodiscard]] virtual std::string_view name() const noexcept = 0;
};

class DoubleOperation final : public Operation {
public:
    [[nodiscard]] int apply(int value) const noexcept override { return value * 2; }
    [[nodiscard]] std::string_view name() const noexcept override { return "double"; }
};

class SquareOperation final : public Operation {
public:
    [[nodiscard]] int apply(int value) const noexcept override { return value * value; }
    [[nodiscard]] std::string_view name() const noexcept override { return "square"; }
};

static_assert(std::has_virtual_destructor_v<Operation>);
static_assert(std::is_abstract_v<Operation>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::unique_ptr<Operation> operation = std::make_unique<DoubleOperation>();
    LEARN_EXPECT_EQ(checks, operation->apply(6), 12);
    LEARN_EXPECT_EQ(checks, operation->name(), std::string_view{"double"});

    operation = std::make_unique<SquareOperation>();
    LEARN_EXPECT_EQ(checks, operation->apply(6), 36);

    // The standard specifies virtual dispatch semantics, not vptr location or
    // vtable slots. Reordering/removing virtuals can break an existing ABI.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section01/vtable_abi_contract", run>;

}  // namespace

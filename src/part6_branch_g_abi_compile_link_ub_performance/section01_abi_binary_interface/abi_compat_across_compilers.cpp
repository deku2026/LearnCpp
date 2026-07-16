// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : abi_compat_across_compilers
// Topic id : part6/g/section01/abi_compat_across_compilers
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part6/g/section01/abi_compat_across_compilers";

class StableWidget {
public:
    StableWidget();
    ~StableWidget();
    StableWidget(StableWidget&&) noexcept;
    StableWidget& operator=(StableWidget&&) noexcept;
    StableWidget(const StableWidget&) = delete;
    StableWidget& operator=(const StableWidget&) = delete;

    void increment() noexcept;
    [[nodiscard]] int value() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

struct StableWidget::Impl {
    int counter{};
    // This private layout may evolve without changing StableWidget's public
    // data-member layout, provided the library preserves its binary contract.
    std::array<int, 8> future_state{};
};

StableWidget::StableWidget() : impl_(std::make_unique<Impl>()) {}
StableWidget::~StableWidget() = default;
StableWidget::StableWidget(StableWidget&&) noexcept = default;
StableWidget& StableWidget::operator=(StableWidget&&) noexcept = default;
void StableWidget::increment() noexcept {
    ++impl_->counter;
}
int StableWidget::value() const noexcept {
    return impl_->counter;
}

struct CAbiHeader {
    std::uint32_t struct_size{};
    std::uint32_t abi_version{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    StableWidget widget;
    widget.increment();
    widget.increment();
    StableWidget moved = std::move(widget);
    LEARN_EXPECT_EQ(checks, moved.value(), 2);
    StableWidget assigned;
    assigned = std::move(moved);
    LEARN_EXPECT_EQ(checks, assigned.value(), 2);

    const CAbiHeader header{sizeof(CAbiHeader), 3};
    LEARN_EXPECT_EQ(checks, header.struct_size, static_cast<std::uint32_t>(sizeof(CAbiHeader)));
    LEARN_EXPECT_EQ(checks, header.abi_version, 3U);

    // Compiler, standard-library, exception, RTTI, packing, and build-mode
    // choices can all be ABI dimensions. Rebuild together or expose a versioned
    // C/POD boundary; never assume std::string layout is portable across them.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section01/abi_compat_across_compilers", run>;

}  // namespace

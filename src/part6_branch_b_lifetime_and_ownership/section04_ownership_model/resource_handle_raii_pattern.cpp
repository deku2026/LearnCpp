// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : resource_handle_raii_pattern
// Topic id : part6/b/section04/resource_handle_raii_pattern
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <ranges>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part6/b/section04/resource_handle_raii_pattern";

class Handle {
public:
    Handle(int value, int& closes) : value_(value), closes_(&closes) {}
    Handle(const Handle&) = delete;
    Handle(Handle&& other) noexcept
        : value_(std::exchange(other.value_, -1)), closes_(std::exchange(other.closes_, nullptr)) {}
    Handle& operator=(Handle&&) = delete;
    ~Handle() { reset(); }
    int get() const noexcept { return value_; }
    void reset() noexcept {
        if (value_ >= 0) {
            ++*closes_;
            value_ = -1;
        }
    }

private:
    int value_;
    int* closes_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    int closes{};
    {
        Handle first{42, closes};
        Handle second{std::move(first)};
        LEARN_EXPECT_EQ(checks, second.get(), 42);
        LEARN_EXPECT_EQ(checks, first.get(), -1);
    }
    LEARN_EXPECT_EQ(checks, closes, 1);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section04/resource_handle_raii_pattern", run>;

}  // namespace

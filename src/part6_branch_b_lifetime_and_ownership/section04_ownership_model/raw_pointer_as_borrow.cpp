// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : raw_pointer_as_borrow
// Topic id : part6/b/section04/raw_pointer_as_borrow
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <memory>
#include <optional>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section04/raw_pointer_as_borrow";

std::optional<int> read(const int* borrowed) {
    if (borrowed == nullptr) return std::nullopt;
    return *borrowed;
}

void increment(int* borrowed) {
    if (borrowed != nullptr) ++*borrowed;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto owner = std::make_unique<int>(7);
    increment(owner.get());
    LEARN_EXPECT_EQ(checks, read(owner.get()).value(), 8);
    LEARN_EXPECT(checks, !read(nullptr));
    LEARN_EXPECT_EQ(checks, *owner, 8);  // The raw pointer did not transfer ownership.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section04/raw_pointer_as_borrow", run>;

}  // namespace

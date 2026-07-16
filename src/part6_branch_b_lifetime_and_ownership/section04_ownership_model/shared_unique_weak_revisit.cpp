// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : shared_unique_weak_revisit
// Topic id : part6/b/section04/shared_unique_weak_revisit
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <memory>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section04/shared_unique_weak_revisit";

struct Node {
    explicit Node(int initial_value) : value(initial_value) {}
    int value;
    std::weak_ptr<Node> parent;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto unique = std::make_unique<Node>(1);
    std::shared_ptr<Node> root = std::move(unique);
    auto child = std::make_shared<Node>(2);
    child->parent = root;
    LEARN_EXPECT(checks, unique == nullptr);
    LEARN_EXPECT_EQ(checks, root.use_count(), 1L);
    LEARN_EXPECT_EQ(checks, child->parent.lock()->value, 1);
    root.reset();
    LEARN_EXPECT(checks, child->parent.expired());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section04/shared_unique_weak_revisit", run>;

}  // namespace

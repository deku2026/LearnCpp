// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : elements_of_recursive_yield
// Topic id : part2/stage12/section05/elements_of_recursive_yield

#ifdef __has_include
#if __has_include(<generator>)
#include <generator>
#define LEARN_HAS_GENERATOR_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <memory>
#include <ranges>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section05/elements_of_recursive_yield";

struct Node {
    int value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

[[nodiscard]] std::unique_ptr<Node> make_node(int value, std::unique_ptr<Node> left = {},
                                              std::unique_ptr<Node> right = {}) {
    return std::make_unique<Node>(Node{value, std::move(left), std::move(right)});
}

void append_inorder(const Node* node, std::vector<int>& output) {
    if (node == nullptr) {
        return;
    }
    append_inorder(node->left.get(), output);
    output.push_back(node->value);
    append_inorder(node->right.get(), output);
}

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L

template <class Range>
concept ElementsOfExpression = requires(Range&& range) { std::ranges::elements_of{std::forward<Range>(range)}; };

std::generator<int> inorder(const Node* node) {
    if (node == nullptr) {
        co_return;
    }
    co_yield std::ranges::elements_of(inorder(node->left.get()));
    co_yield node->value;
    co_yield std::ranges::elements_of(inorder(node->right.get()));
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    auto tree = make_node(4, make_node(2, make_node(1), make_node(3)), make_node(6, make_node(5), make_node(7)));
    const std::vector expected{1, 2, 3, 4, 5, 6, 7};
    std::vector<int> fallback;
    append_inorder(tree.get(), fallback);
    LEARN_EXPECT_EQ(checks, fallback, expected);

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    // P2502 supplies generator and ranges::elements_of under the same feature macro.
    static_assert(ElementsOfExpression<std::generator<int>>);
    std::vector<int> actual;
    for (const int value : inorder(tree.get())) {
        actual.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, actual, expected);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 generator/elements_of (__cpp_lib_generator >= 202207L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/elements_of_recursive_yield", run>;

}  // namespace

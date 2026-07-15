// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : elements_of_recursive_yield
// Topic id : part2/stage12/section05/elements_of_recursive_yield
//
// 要点: co_yield std::ranges::elements_of(inner) 递归产出内层全部元素;
//       标准实现用对称转移, 每元素 O(1) 转移(非逐层 resume)。
// Refs:
//   https://en.cppreference.com/w/cpp/ranges/elements_of
//   https://en.cppreference.com/w/cpp/coroutine/generator

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
#include <generator>
#define LEARN_HAS_STD_GENERATOR 1
#else
#define LEARN_HAS_STD_GENERATOR 0
#include <coroutine>
#endif

namespace {

struct TreeNode {
    int value{};
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

std::unique_ptr<TreeNode> make(int v, std::unique_ptr<TreeNode> l = nullptr, std::unique_ptr<TreeNode> r = nullptr) {
    auto n = std::make_unique<TreeNode>();
    n->value = v;
    n->left = std::move(l);
    n->right = std::move(r);
    return n;
}

#if LEARN_HAS_STD_GENERATOR

// ⭐ 中序遍历: elements_of 递归 yield 子树
std::generator<int> inorder(const TreeNode* node) {
    if (!node) {
        co_return;
    }
    co_yield std::ranges::elements_of(inorder(node->left.get()));
    co_yield node->value;
    co_yield std::ranges::elements_of(inorder(node->right.get()));
}

// 对比: 朴素 for+co_yield 也能正确, 但深度大时每元素 O(深度) resume
std::generator<int> inorder_naive(const TreeNode* node) {
    if (!node) {
        co_return;
    }
    for (int x : inorder_naive(node->left.get())) {
        co_yield x;
    }
    co_yield node->value;
    for (int x : inorder_naive(node->right.get())) {
        co_yield x;
    }
}

#else

template <typename T>
struct FallbackGen {
    struct promise_type {
        T current{};
        FallbackGen get_return_object() {
            return FallbackGen{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            current = std::move(v);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit FallbackGen(Handle h) noexcept : h_(h) {}
    ~FallbackGen() {
        if (h_) {
            h_.destroy();
        }
    }
    FallbackGen(FallbackGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    FallbackGen(const FallbackGen&) = delete;
    struct iterator {
        Handle h{};
        void operator++() { h.resume(); }
        const T& operator*() const { return h.promise().current; }
        bool operator==(std::default_sentinel_t) const { return !h || h.done(); }
    };
    iterator begin() {
        if (h_) {
            h_.resume();
        }
        return iterator{h_};
    }
    std::default_sentinel_t end() const noexcept { return {}; }

private:
    Handle h_{};
};

FallbackGen<int> inorder(const TreeNode* node) {
    if (!node) {
        co_return;
    }
    for (int x : inorder(node->left.get())) {
        co_yield x;
    }
    co_yield node->value;
    for (int x : inorder(node->right.get())) {
        co_yield x;
    }
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== elements_of_recursive_yield ===\n";

    //         4
    //       /   \
    //      2     6
    //     / \   / \
    //    1   3 5   7
    auto root = make(4, make(2, make(1), make(3)), make(6, make(5), make(7)));

    std::vector<int> got;
    for (int x : inorder(root.get())) {
        got.push_back(x);
    }
    assert((got == std::vector<int>{1, 2, 3, 4, 5, 6, 7}));
    std::cout << "  inorder:";
    for (int x : got) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';

#if LEARN_HAS_STD_GENERATOR
    std::vector<int> naive;
    for (int x : inorder_naive(root.get())) {
        naive.push_back(x);
    }
    assert(naive == got);
    std::cout << "  elements_of vs naive for-loop: same values;"
                 " elements_of uses symmetric transfer (O(1)/elem)\n";
#else
    std::cout << "  (no std::generator: naive recursive yield only)\n";
#endif

    std::cout << "elements_of_recursive_yield: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/elements_of_recursive_yield", run>;

}  // namespace

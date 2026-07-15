// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : variadic_perfect_forwarding
// Topic id : part2/stage06/section02/variadic_perfect_forwarding
//
// Covers: make_unique-style factory with pack + std::forward

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <utility>

namespace {

struct Widget {
    int a = 0;
    std::string b;
    Widget(int x, std::string y) : a(x), b(std::move(y)) {}
};

template <typename T, typename... Args>
std::unique_ptr<T> my_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename F, typename... Args>
decltype(auto) invoke_forward(F&& f, Args&&... args) {
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

void demo_basics() {
    auto p = my_make_unique<Widget>(7, std::string{"hi"});
    assert(p != nullptr);
    assert(p->a == 7);
    assert(p->b == "hi");
}

void demo_intermediate() {
    std::string s = "owned";
    auto p = my_make_unique<Widget>(1, std::move(s));
    assert(p->b == "owned");
    // s is valid but unspecified after move; safe to assign.
    s = "reset";
    assert(s == "reset");
}

void demo_expert() {
    int calls = 0;
    auto f = [&](int x, int y) {
        ++calls;
        return x + y;
    };
    assert(invoke_forward(f, 2, 3) == 5);
    assert(calls == 1);

    auto g = [](std::string&& s) { return s.size(); };
    assert(invoke_forward(g, std::string{"abcd"}) == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/variadic_perfect_forwarding", run>;

}  // namespace

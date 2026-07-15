// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : std_forward_usage
// Topic id : part2/stage05/section02/std_forward_usage
//
// Covers: std::forward preserves value category; bare arg loses rvalueness

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <utility>

namespace {

int g_lvalue_hits = 0;
int g_rvalue_hits = 0;

void target(std::string&) {
    ++g_lvalue_hits;
}
void target(std::string&&) {
    ++g_rvalue_hits;
}

template <class T>
void wrapper_forward(T&& arg) {
    target(std::forward<T>(arg));
}

template <class T>
void wrapper_broken(T&& arg) {
    target(arg);  // arg is always an lvalue expression
}

void demo_basics() {
    g_lvalue_hits = 0;
    g_rvalue_hits = 0;

    std::string s = "hi";
    wrapper_forward(s);
    wrapper_forward(std::string{});
    assert(g_lvalue_hits == 1);
    assert(g_rvalue_hits == 1);
}

void demo_intermediate() {
    g_lvalue_hits = 0;
    g_rvalue_hits = 0;

    std::string s = "x";
    wrapper_broken(s);
    wrapper_broken(std::string{});
    assert(g_lvalue_hits == 2);
    assert(g_rvalue_hits == 0);  // rvalue category lost without forward
}

void demo_expert() {
    // move for rvalue-ref params; forward for forwarding refs
    auto consume_rref = [](std::string&& s) {
        std::string local = std::move(s);
        return local;
    };
    assert(consume_rref(std::string{"ok"}) == "ok");

    auto factory = [](auto&& s) { return std::string(std::forward<decltype(s)>(s)); };
    std::string a = "copy-me";
    std::string b = factory(a);
    std::string c = factory(std::string{"move-me"});
    assert(b == "copy-me");
    assert(c == "move-me");
    assert(a == "copy-me");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/std_forward_usage", run>;

}  // namespace

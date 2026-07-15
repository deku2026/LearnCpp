// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : variadic_pack_forwarding
// Topic id : part2/stage05/section02/variadic_pack_forwarding
//
// Covers: Args&&... + std::forward<Args>(args)... for emplace/make patterns

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Widget {
    int id;
    std::string name;

    Widget(int i, std::string n) : id(i), name(std::move(n)) {}
};

template <class T, class... Args>
std::unique_ptr<T> my_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class... Args>
std::string join_forwarded(Args&&... args) {
    std::string out;
    // Fold: append stringified pieces (simple demo for pack expansion)
    ((out += std::forward<Args>(args)), ...);
    return out;
}

void demo_basics() {
    auto p = my_make_unique<Widget>(1, std::string{"w"});
    assert(p != nullptr);
    assert(p->id == 1);
    assert(p->name == "w");
}

void demo_intermediate() {
    std::vector<Widget> v;
    std::string name = "alpha";
    v.emplace_back(7, name);                 // lvalue string -> copy into Widget
    v.emplace_back(8, std::string{"beta"});  // rvalue string -> move into Widget
    assert(v.size() == 2);
    assert(v[0].name == "alpha");
    assert(v[1].name == "beta");
    assert(name == "alpha");
}

void demo_expert() {
    std::string a = "A";
    std::string b = "B";
    auto s = join_forwarded(a, std::string{"-"}, std::move(b), "!");
    assert(s == "A-B!");
    assert(a == "A");

    auto q = std::make_unique<Widget>(42, "std");
    assert(q->id == 42);
    assert(q->name == "std");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/variadic_pack_forwarding", run>;

}  // namespace

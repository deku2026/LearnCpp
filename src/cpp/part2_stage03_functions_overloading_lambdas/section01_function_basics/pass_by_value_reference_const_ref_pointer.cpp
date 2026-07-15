// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : pass_by_value_reference_const_ref_pointer
// Topic id : part2/stage03/section01/pass_by_value_reference_const_ref_pointer
//
// Covers: by-value, T&, const T&, T*; sink-by-value + move preview

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <utility>

namespace {

void by_value(std::string s) {
    s += "!";
}

void by_ref(std::string& s) {
    s += "!";
}

void by_const_ref(const std::string& s) {
    assert(!s.empty());
}

void by_ptr(std::string* s) {
    if (s != nullptr) {
        *s += "?";
    }
}

struct Widget {
    std::string name_;
    void set_name(std::string name) { name_ = std::move(name); }
};

void demo_basics() {
    std::string a = "alpha";
    by_value(a);
    assert(a == "alpha");

    by_ref(a);
    assert(a == "alpha!");

    by_const_ref(a);
    by_const_ref(std::string{"temp"});

    by_ptr(&a);
    assert(a == "alpha!?");
    by_ptr(nullptr);
}

void demo_intermediate() {
    int n = 7;
    auto bump = [](int x) { return x + 1; };
    assert(bump(n) == 8);
    assert(n == 7);

    auto bump_ref = [](int& x) { ++x; };
    bump_ref(n);
    assert(n == 8);

    const std::string big = "read-only payload";
    by_const_ref(big);
}

void demo_expert() {
    Widget w;
    std::string local = "owned";
    w.set_name(local);
    assert(w.name_ == "owned");
    assert(local == "owned");

    w.set_name(std::string{"rvalue"});
    assert(w.name_ == "rvalue");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage03/section01/pass_by_value_reference_const_ref_pointer", run>;

}  // namespace

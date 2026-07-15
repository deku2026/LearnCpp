// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : forwarding_reference_universal
// Topic id : part2/stage05/section02/forwarding_reference_universal
//
// Covers: T&& with deduced T is forwarding reference; binds lvalues and rvalues

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

template <class T>
constexpr bool is_lvalue_ref_param(T&&) {
    return std::is_lvalue_reference_v<T>;
}

template <class T>
void only_rvalue(std::vector<T>&&) {}  // not a forwarding ref: T not deduced as ref form

template <class T>
std::string kind_of(T&&) {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return "lvalue";
    } else {
        return "rvalue";
    }
}

void demo_basics() {
    int x = 1;
    LEARN_CHECK(is_lvalue_ref_param(x));
    LEARN_CHECK(!is_lvalue_ref_param(1));
    LEARN_CHECK(!is_lvalue_ref_param(std::move(x)));
}

void demo_intermediate() {
    std::string s = "hi";
    LEARN_CHECK(kind_of(s) == "lvalue");
    LEARN_CHECK(kind_of(std::string{"tmp"}) == "rvalue");
    LEARN_CHECK(kind_of(std::move(s)) == "rvalue");

    std::vector<int> v{1, 2, 3};
    only_rvalue(std::move(v));
    // only_rvalue(v); // would not compile: needs rvalue vector
}

void demo_expert() {
    // Deduction: lvalue -> T = U&, rvalue -> T = U
    auto probe = [](auto&& arg) {
        using A = decltype(arg);
        if constexpr (std::is_lvalue_reference_v<A>) {
            return 1;
        } else {
            return 2;
        }
    };
    int n = 0;
    LEARN_CHECK(probe(n) == 1);
    LEARN_CHECK(probe(0) == 2);

    static_assert(std::is_same_v<decltype(std::declval<int&>()), int&>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/forwarding_reference_universal", run>;

}  // namespace

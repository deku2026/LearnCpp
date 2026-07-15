// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : static_initialization_order_problem
// Topic id : part6/b/section01/static_initialization_order_problem
//
// Covers: SIOF: undefined order across TUs; fix with function-local statics

#include "learn/topic_registry.hpp"

#include <string>

namespace {

// Simulated two "TUs" as namespaces — cross-TU order is not portable.
namespace tu_a {
inline std::string& registry() {
    static std::string r = "ok";
    return r;
}
}  // namespace tu_a

namespace tu_b {
inline int use_registry() {
    return static_cast<int>(tu_a::registry().size());
}
}  // namespace tu_b

void demo_basics() {
    // Function-local static: initialized on first control pass (C++11 thread-safe).
    LEARN_CHECK(tu_a::registry() == "ok");
}

void demo_intermediate() {
    LEARN_CHECK(tu_b::use_registry() == 2);
}

void demo_expert() {
    // Teaching note: namespace-scope dynamic init order across TUs is unspecified.
    // Prefer Meyers singleton / function-local static or constexpr data.
    static int a = 1;
    static int b = a + 1;  // same TU: ordered
    LEARN_CHECK(b == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/static_initialization_order_problem", run>;

}  // namespace

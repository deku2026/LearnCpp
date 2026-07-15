// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section05_crtp_and_policy_based_design
// Item     : crtp_deducing_this_simplification_cpp23
// Topic id : part6/e/section05/crtp_deducing_this_simplification_cpp23
//
// Covers: deducing this simplifies CRTP (C++23)

#include "learn/topic_registry.hpp"

namespace {

struct Printer {
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    template <class Self>
    int id(this Self&& self) {
        return self.value;
    }
    int value = 0;
#else
    int value = 0;
    int id() const { return value; }
#endif
};

void demo_basics() {
    Printer p;
    p.value = 5;
    LEARN_CHECK(p.id() == 5);
}

void demo_intermediate() {
    const Printer p{7};
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    LEARN_CHECK(p.id() == 7);
#else
    LEARN_CHECK(p.id() == 7);
#endif
}

void demo_expert() {
    // deducing this can replace many CRTP mixins with a single member template.
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section05/crtp_deducing_this_simplification_cpp23", run>;

}  // namespace

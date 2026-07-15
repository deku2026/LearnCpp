// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section05_crtp_and_policy_based_design
// Item     : policy_based_design
// Topic id : part6/e/section05/policy_based_design
//
// Covers: policy-based design with template policies

#include "learn/topic_registry.hpp"

#include <string>

namespace {

struct FastPolicy {
    static std::string name() { return "fast"; }
    static int scale(int x) { return x; }
};

struct SafePolicy {
    static std::string name() { return "safe"; }
    static int scale(int x) { return x * 2; }
};

template <class Policy>
struct Engine {
    int run(int x) const { return Policy::scale(x); }
    std::string kind() const { return Policy::name(); }
};

void demo_basics() {
    Engine<FastPolicy> e;
    LEARN_CHECK(e.run(3) == 3);
    LEARN_CHECK(e.kind() == "fast");
}

void demo_intermediate() {
    Engine<SafePolicy> e;
    LEARN_CHECK(e.run(3) == 6);
}

void demo_expert() {
    LEARN_CHECK(Engine<SafePolicy>{}.kind() == "safe");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section05/policy_based_design", run>;

}  // namespace

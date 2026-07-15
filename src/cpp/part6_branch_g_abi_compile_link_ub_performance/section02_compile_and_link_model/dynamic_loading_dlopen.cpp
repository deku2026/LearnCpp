// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : dynamic_loading_dlopen
// Topic id : part6/g/section02/dynamic_loading_dlopen
//
// Covers: dynamic loading concept without requiring real dlopen

#include "learn/topic_registry.hpp"

#include <string>

namespace {

struct Plugin {
    const char* name;
    int (*run)(int);
};

int plugin_run(int x) {
    return x + 1;
}

Plugin load_plugin() {
    return Plugin{"demo", &plugin_run};
}

void demo_basics() {
    Plugin p = load_plugin();
    LEARN_CHECK(std::string{p.name} == "demo");
    LEARN_CHECK(p.run(3) == 4);
}

void demo_intermediate() {
    Plugin p = load_plugin();
    LEARN_CHECK(p.run(0) == 1);
}

void demo_expert() {
    Plugin p = load_plugin();
    LEARN_CHECK(p.run != nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/dynamic_loading_dlopen", run>;

}  // namespace

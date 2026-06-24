// LearnCpp placeholder
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : multiple_inheritance_multiple_vptr
// Topic id : part6/a/section02/multiple_inheritance_multiple_vptr
//
// TODO: read cppreference, sketch a minimal example, check godbolt / C++ Insights,
//       then replace this empty run() body with real demo code.

#include "learn/topic_registry.hpp"

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/multiple_inheritance_multiple_vptr", run>;

}  // namespace

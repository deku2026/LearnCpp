// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : basic_common_reference_for_reference_wrapper_cpp23
// Topic id : part6/e/section10/basic_common_reference_for_reference_wrapper_cpp23
//
// Covers: common_reference and reference_wrapper interactions

#include "learn/topic_registry.hpp"

#include <functional>
#include <type_traits>

namespace {

void demo_basics() {
    int x = 1;
    std::reference_wrapper<int> r = x;
    LEARN_CHECK(r.get() == 1);
    r.get() = 2;
    LEARN_CHECK(x == 2);
}

void demo_intermediate() {
    using CR = std::common_reference_t<int&, int&>;
    static_assert(std::is_same_v<CR, int&>);
}

void demo_expert() {
    int a = 3, b = 4;
    auto ra = std::ref(a);
    auto rb = std::ref(b);
    LEARN_CHECK(ra.get() + rb.get() == 7);
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
    ::learn::topic<"part6/e/section10/basic_common_reference_for_reference_wrapper_cpp23", run>;

}  // namespace

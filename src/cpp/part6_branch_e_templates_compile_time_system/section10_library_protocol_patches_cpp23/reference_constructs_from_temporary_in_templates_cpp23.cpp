// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : reference_constructs_from_temporary_in_templates_cpp23
// Topic id : part6/e/section10/reference_constructs_from_temporary_in_templates_cpp23
//
// Covers: reject binding references to temporaries in templates

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

template <class T>
constexpr bool safe_const_ref_param() {
#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    return !std::reference_constructs_from_temporary_v<const int&, T>;
#else
    return !std::is_rvalue_reference_v<T&&> || std::is_lvalue_reference_v<T>;
#endif
}

void demo_basics() {
    int x = 1;
    const int& r = x;
    LEARN_CHECK(r == 1);
}

void demo_intermediate() {
#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    static_assert(!std::reference_constructs_from_temporary_v<const int&, int&>);
#endif
    LEARN_CHECK(true);
}

void demo_expert() {
    LEARN_CHECK(safe_const_ref_param<int&>() || !safe_const_ref_param<int&>());
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
    ::learn::topic<"part6/e/section10/reference_constructs_from_temporary_in_templates_cpp23", run>;

}  // namespace

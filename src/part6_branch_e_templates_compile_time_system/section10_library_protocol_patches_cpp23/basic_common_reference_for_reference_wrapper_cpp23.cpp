// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : basic_common_reference_for_reference_wrapper_cpp23
// Topic id : part6/e/section10/basic_common_reference_for_reference_wrapper_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <concepts>
#include <functional>
#include <type_traits>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section10/basic_common_reference_for_reference_wrapper_cpp23"};

    int value = 40;
    std::reference_wrapper<int> wrapper{value};
    wrapper.get() += 2;
    LEARN_EXPECT_EQ(checks, value, 42);

#if defined(__cpp_lib_common_reference_wrapper) && __cpp_lib_common_reference_wrapper >= 202302L
    using Common = std::common_reference_t<std::reference_wrapper<int>, int&>;
    static_assert(std::same_as<Common, int&>);
    Common common = wrapper;
    common = 7;
    LEARN_EXPECT_EQ(checks, value, 7);
#else
    // unwrap_reference is the conservative pre-C++23 bridge, but it does not
    // retrofit the complete common_reference protocol used by Ranges.
    using Unwrapped = std::unwrap_reference_t<std::reference_wrapper<int>>;
    static_assert(std::same_as<Unwrapped, int&>);
    LEARN_EXPECT_EQ(checks, wrapper.get(), 42);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/e/section10/basic_common_reference_for_reference_wrapper_cpp23", run>;

}  // namespace

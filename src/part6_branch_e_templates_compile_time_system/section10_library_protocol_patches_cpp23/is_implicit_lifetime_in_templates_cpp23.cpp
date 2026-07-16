// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : is_implicit_lifetime_in_templates_cpp23
// Topic id : part6/e/section10/is_implicit_lifetime_in_templates_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <string_view>
#include <type_traits>
#include <version>

namespace {

struct TrivialRecord {
    int id;
    double value;
};

template <class T>
inline constexpr bool conservative_raw_storage_candidate =
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    std::is_implicit_lifetime_v<T>;
#else
    std::is_trivially_copyable_v<T>;
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section10/is_implicit_lifetime_in_templates_cpp23"};

    static_assert(conservative_raw_storage_candidate<int>);
    static_assert(conservative_raw_storage_candidate<TrivialRecord>);
    static_assert(std::is_trivially_copyable_v<TrivialRecord>);
    LEARN_EXPECT(checks, conservative_raw_storage_candidate<TrivialRecord>);

#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    constexpr std::string_view status{"std::is_implicit_lifetime is available"};
#else
    constexpr std::string_view status{"using a stricter trivially-copyable fallback"};
#endif
    LEARN_EXPECT(checks, !status.empty());
    // The trait is only a prerequisite: alignment, storage size, and the
    // relevant lifetime-starting operation must still be correct.

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section10/is_implicit_lifetime_in_templates_cpp23", run>;

}  // namespace

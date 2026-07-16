// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : reference_constructs_from_temporary_cpp23
// Topic id : part6/b/section05/reference_constructs_from_temporary_cpp23
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/b/section05/reference_constructs_from_temporary_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L && \
    (!defined(LEARNCPP_HAS_REFERENCE_FROM_TEMPORARY_TRAITS) || LEARNCPP_HAS_REFERENCE_FROM_TEMPORARY_TRAITS)
    ::learn::ExampleChecks checks{kTopic};
    static_assert(std::reference_constructs_from_temporary_v<const std::string&, std::string>);
    static_assert(!std::reference_constructs_from_temporary_v<const std::string&, std::string&>);
    LEARN_EXPECT(checks, (std::reference_constructs_from_temporary_v<const std::string&, std::string>));
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "reference_constructs_from_temporary");
#endif
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part6/b/section05/reference_constructs_from_temporary_cpp23", run>;

}  // namespace

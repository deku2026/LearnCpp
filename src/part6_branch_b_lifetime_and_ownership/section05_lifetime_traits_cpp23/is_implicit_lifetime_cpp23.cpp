// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : is_implicit_lifetime_cpp23
// Topic id : part6/b/section05/is_implicit_lifetime_cpp23
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <version>

namespace {

constexpr std::string_view kTopic = "part6/b/section05/is_implicit_lifetime_cpp23";

struct TrivialRecord {
    int value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    ::learn::ExampleChecks checks{kTopic};
    static_assert(std::is_implicit_lifetime_v<int>);
    static_assert(std::is_implicit_lifetime_v<TrivialRecord>);
    static_assert(!std::is_implicit_lifetime_v<std::string>);
    LEARN_EXPECT(checks, std::is_implicit_lifetime_v<TrivialRecord>);
    return checks.result();
#else
    static_assert(std::is_trivially_copyable_v<TrivialRecord>);
    return ::learn::ExampleChecks::unavailable(kTopic, "std::is_implicit_lifetime");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section05/is_implicit_lifetime_cpp23", run>;

}  // namespace

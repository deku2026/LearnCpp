// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : type_support_headers
// Topic id : part3/section08/type_support_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

namespace {

constexpr std::string_view kTopic = "part3/section08/type_support_headers";

constexpr std::array<std::string_view, 10> headers{
    "compare",     "concepts",  "cstddef",  "cstdint", "limits",
    "type_traits", "typeindex", "typeinfo", "version", "source_location",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    static_assert(std::integral<std::int32_t>);
    static_assert(std::same_as<std::remove_cvref_t<const int&>, int>);
    const std::type_index index{typeid(int)};
    LEARN_EXPECT(checks, index == std::type_index{typeid(int)});
    LEARN_EXPECT(checks, std::numeric_limits<std::uint32_t>::max() > 0U);
    LEARN_EXPECT_EQ(checks, headers.size(), 10U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/type_support_headers", run>;

}  // namespace

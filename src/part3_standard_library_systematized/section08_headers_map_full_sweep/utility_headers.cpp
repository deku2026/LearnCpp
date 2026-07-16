// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : utility_headers
// Topic id : part3/section08/utility_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <any>
#include <array>
#include <functional>
#include <optional>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>

namespace {

constexpr std::string_view kTopic = "part3/section08/utility_headers";

constexpr std::array<std::string_view, 12> headers{
    "any",     "expected", "functional",  "optional", "tuple",   "utility",
    "variant", "memory",   "type_traits", "concepts", "compare", "version",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::optional<int> optional{7};
    const std::variant<int, std::string_view> variant{std::string_view{"ready"}};
    const std::any any = 23;
    const auto pair = std::pair{2, 3};
    LEARN_EXPECT_EQ(checks, optional.value(), 7);
    LEARN_EXPECT_EQ(checks, std::get<std::string_view>(variant), std::string_view{"ready"});
    LEARN_EXPECT_EQ(checks, std::any_cast<int>(any), 23);
    LEARN_EXPECT_EQ(checks, pair.first + pair.second, 5);
    LEARN_EXPECT_EQ(checks, headers.size(), 12U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/utility_headers", run>;

}  // namespace

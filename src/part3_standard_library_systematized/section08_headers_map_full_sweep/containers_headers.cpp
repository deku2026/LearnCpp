// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : containers_headers
// Topic id : part3/section08/containers_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <map>
#include <span>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section08/containers_headers";

constexpr std::array<std::string_view, 16> headers{
    "array", "deque", "flat_map", "flat_set", "forward_list",  "list",          "map",    "mdspan",
    "queue", "set",   "span",     "stack",    "unordered_map", "unordered_set", "vector", "initializer_list",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> sequence{1, 2, 3};
    const std::span view{sequence};
    const std::map<int, int> ordered{{2, 4}, {1, 1}};
    const std::unordered_set<int> hashed{3, 3, 4};
    LEARN_EXPECT_EQ(checks, view.size(), 3U);
    LEARN_EXPECT_EQ(checks, ordered.begin()->first, 1);
    LEARN_EXPECT_EQ(checks, hashed.size(), 2U);
    LEARN_EXPECT_EQ(checks, headers.size(), 16U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/containers_headers", run>;

}  // namespace

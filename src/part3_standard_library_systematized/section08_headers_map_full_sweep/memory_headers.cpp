// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : memory_headers
// Topic id : part3/section08/memory_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <memory>
#include <memory_resource>
#include <new>
#include <scoped_allocator>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section08/memory_headers";

constexpr std::array<std::string_view, 6> headers{
    "memory", "memory_resource", "new", "scoped_allocator", "cstddef", "cstdlib",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto owner = std::make_shared<int>(23);
    std::weak_ptr<int> observer = owner;
    LEARN_EXPECT_EQ(checks, *observer.lock(), 23);
    std::array<std::byte, 256> buffer{};
    std::pmr::monotonic_buffer_resource resource{buffer.data(), buffer.size()};
    std::pmr::vector<int> values{&resource};
    values.assign({1, 2, 3});
    LEARN_EXPECT_EQ(checks, values.size(), 3U);
    LEARN_EXPECT_EQ(checks, headers.size(), 6U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/memory_headers", run>;

}  // namespace

// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : start_lifetime_as_array_cpp23
// Topic id : part6/c/section04/start_lifetime_as_array_cpp23
// Reference: WG21 P2590R2 (__cpp_lib_start_lifetime_as).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section04/start_lifetime_as_array_cpp23";
    learn::ExampleChecks checks{topic};
    constexpr std::array<std::uint32_t, 4> source{2U, 3U, 5U, 7U};

#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L
    alignas(std::uint32_t) std::array<std::byte, sizeof(source)> representation{};
    std::memcpy(representation.data(), source.data(), sizeof(source));
    std::uint32_t* values = std::start_lifetime_as_array<std::uint32_t>(representation.data(), source.size());
    for (std::size_t index = 0; index < source.size(); ++index) {
        LEARN_EXPECT_EQ(checks, values[index], source[index]);
    }
    LEARN_EXPECT_EQ(checks, reinterpret_cast<std::uintptr_t>(values) % alignof(std::uint32_t), std::uintptr_t{0});
#else
    std::array<std::uint32_t, 4> values{};
    std::memcpy(values.data(), source.data(), sizeof(source));
    LEARN_EXPECT_EQ(checks, values, source);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_start_lifetime_as >= 202207L");
#endif
    // Count must fit the provided region; alignment, reachability and implicit-lifetime type
    // preconditions remain the caller's responsibility.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/start_lifetime_as_array_cpp23", run>;

}  // namespace

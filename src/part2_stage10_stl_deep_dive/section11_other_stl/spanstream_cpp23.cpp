// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : spanstream_cpp23
// Topic id : part2/stage10/section11/spanstream_cpp23
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<spanstream>)
#include <spanstream>
#endif
#include <array>
#include <span>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/spanstream_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
    ::learn::ExampleChecks checks{kTopic};
    std::array<char, 32> storage{};
    std::ospanstream output{std::span<char>{storage}};
    output << "value " << 42;
    const std::span<const char> written = output.span();
    LEARN_EXPECT_EQ(checks, (std::string_view{written.data(), written.size()}), std::string_view{"value 42"});

    std::ispanstream input{written};
    std::string word;
    int number{};
    input >> word >> number;
    LEARN_EXPECT_EQ(checks, word, std::string{"value"});
    LEARN_EXPECT_EQ(checks, number, 42);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::spanstream");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section11/spanstream_cpp23", run>;

}  // namespace

// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section12_coroutine_library
// Item     : std_generator_cpp23_recap
// Topic id : part2/stage10/section12/std_generator_cpp23_recap
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<generator>)
#include <generator>
#endif
#include <vector>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section12/std_generator_cpp23_recap";

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
std::generator<int> squares(int count) {
    for (int value = 0; value < count; ++value) {
        co_yield value* value;
    }
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values;
    for (const int value : squares(5)) {
        values.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({0, 1, 4, 9, 16}));
    LEARN_EXPECT(checks, squares(0).begin() == squares(0).end());
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::generator");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section12/std_generator_cpp23_recap", run>;

}  // namespace

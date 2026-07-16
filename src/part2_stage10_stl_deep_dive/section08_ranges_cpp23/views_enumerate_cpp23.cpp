// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_enumerate_cpp23
// Topic id : part2/stage10/section08/views_enumerate_cpp23
// Reference: WG21 P2164R9 (views::enumerate), macro value adopted after C++23 publication.

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<ranges>)
#include <ranges>
#define LEARN_HAS_RANGES_HEADER 1
#else
#define LEARN_HAS_RANGES_HEADER 0
#endif

#include <string>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_enumerate_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector<std::string> names{"zero", "one", "two"};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_enumerate) && __cpp_lib_ranges_enumerate >= 202302L
    auto indexed = names | std::views::enumerate;
    std::vector<std::string> labels;
    for (auto [index, name] : indexed) {
        labels.push_back(std::to_string(index) + ':' + name);
        if (index == 1) {
            name = "ONE";  // The tuple's second component preserves the source reference.
        }
    }
    LEARN_EXPECT_EQ(checks, labels, (std::vector<std::string>{"0:zero", "1:one", "2:two"}));
    LEARN_EXPECT_EQ(checks, names[1], std::string{"ONE"});
    LEARN_EXPECT_EQ(checks, indexed.size(), names.size());
    // Indices start at zero and use range_difference_t; no extra index container is allocated.
#else
    std::vector<std::string> labels;
    for (std::size_t index = 0; index < names.size(); ++index) {
        labels.push_back(std::to_string(index) + ':' + names[index]);
    }
    LEARN_EXPECT_EQ(checks, labels.front(), std::string{"0:zero"});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_enumerate >= 202302L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_enumerate_cpp23", run>;

}  // namespace

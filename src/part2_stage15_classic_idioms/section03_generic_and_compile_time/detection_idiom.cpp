// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : detection_idiom
// Topic id : part2/stage15/section03/detection_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section03/detection_idiom";

template <class, class = void>
struct has_size : std::false_type {};

template <class T>
struct has_size<T, std::void_t<decltype(std::declval<const T&>().size())>> : std::true_type {};

template <class T>
inline constexpr bool has_size_v = has_size<T>::value;

static_assert(has_size_v<std::string>);
static_assert(!has_size_v<int>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::string text = "detect";
    if constexpr (has_size_v<decltype(text)>) LEARN_EXPECT_EQ(checks, text.size(), 6U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section03/detection_idiom", run>;

}  // namespace

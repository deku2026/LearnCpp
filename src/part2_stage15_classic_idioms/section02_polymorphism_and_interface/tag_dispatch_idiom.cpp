// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : tag_dispatch_idiom
// Topic id : part2/stage15/section02/tag_dispatch_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <forward_list>
#include <iterator>
#include <list>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section02/tag_dispatch_idiom";

template <class Iterator>
std::string_view strategy(Iterator, std::random_access_iterator_tag) {
    return "constant-time jump";
}

template <class Iterator>
std::string_view strategy(Iterator, std::input_iterator_tag) {
    return "incremental walk";
}

template <class Iterator>
std::string_view strategy(Iterator iterator) {
    using Category = typename std::iterator_traits<Iterator>::iterator_category;
    return strategy(iterator, Category{});
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> vector{1};
    std::forward_list<int> forward{1};
    LEARN_EXPECT_EQ(checks, strategy(vector.begin()), std::string_view{"constant-time jump"});
    LEARN_EXPECT_EQ(checks, strategy(forward.begin()), std::string_view{"incremental walk"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section02/tag_dispatch_idiom", run>;

}  // namespace

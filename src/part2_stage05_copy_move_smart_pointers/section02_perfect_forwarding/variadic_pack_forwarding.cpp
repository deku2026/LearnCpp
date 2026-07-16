// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : variadic_pack_forwarding
// Topic id : part2/stage05/section02/variadic_pack_forwarding
// References: [temp.variadic], [forward]

#include "learn/example_support.hpp"

#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section02/variadic_pack_forwarding";

template <class... Arguments>
auto owning_tuple(Arguments&&... arguments) {
    return std::tuple<std::decay_t<Arguments>...>{std::forward<Arguments>(arguments)...};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::string label = "copied lvalue";
    auto values = owning_tuple(label, 7, std::make_unique<int>(9));

    LEARN_EXPECT_EQ(checks, std::get<0>(values), std::string{"copied lvalue"});
    LEARN_EXPECT_EQ(checks, std::get<1>(values), 7);
    LEARN_EXPECT_EQ(checks, *std::get<2>(values), 9);
    label = "changed";
    LEARN_EXPECT_EQ(checks, std::get<0>(values), std::string{"copied lvalue"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section02/variadic_pack_forwarding", run>;

}  // namespace

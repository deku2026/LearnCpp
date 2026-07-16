// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : forward_like_cpp23
// Topic id : part2/stage05/section02/forward_like_cpp23
// References: [forward], P2445R1, SD-FeatureTest __cpp_lib_forward_like

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section02/forward_like_cpp23";

struct Owner {
    int value{};
};

template <class Like, class T>
decltype(auto) member_like(T& value) noexcept {
#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
    return std::forward_like<Like>(value);
#else
    using Value = std::remove_reference_t<T>;
    constexpr bool as_const = std::is_const_v<std::remove_reference_t<Like>>;
    constexpr bool as_lvalue = std::is_lvalue_reference_v<Like&&>;
    if constexpr (as_lvalue && as_const) {
        return static_cast<const Value&>(value);
    } else if constexpr (as_lvalue) {
        return static_cast<Value&>(value);
    } else if constexpr (as_const) {
        return static_cast<const Value&&>(std::move(value));
    } else {
        return static_cast<Value&&>(std::move(value));
    }
#endif
}

static_assert(std::is_same_v<decltype(member_like<Owner&>(std::declval<int&>())), int&>);
static_assert(std::is_same_v<decltype(member_like<const Owner&>(std::declval<int&>())), const int&>);
static_assert(std::is_same_v<decltype(member_like<Owner&&>(std::declval<int&>())), int&&>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int value = 4;
    member_like<Owner&>(value) = 8;
    const int& read_only = member_like<const Owner&>(value);
    LEARN_EXPECT_EQ(checks, value, 8);
    LEARN_EXPECT_EQ(checks, read_only, 8);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section02/forward_like_cpp23", run>;

}  // namespace

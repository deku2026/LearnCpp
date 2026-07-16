// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : cpo_customization_point
// Topic id : part2/stage15/section03/cpo_customization_point
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section03/cpo_customization_point";

struct describe_t {
    template <class T>
        requires requires(const T& value) { tag_invoke(std::declval<describe_t>(), value); }
    std::string operator()(const T& value) const {
        return tag_invoke(*this, value);
    }
};

inline constexpr describe_t describe{};

template <class T>
concept Describable = requires(const T& value) { describe(value); };

struct User {
    std::string name;
    friend std::string tag_invoke(describe_t, const User& user) { return "user:" + user.name; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, describe(User{"Ada"}), std::string{"user:Ada"});
    static_assert(Describable<User>);
    static_assert(!Describable<int>);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section03/cpo_customization_point", run>;

}  // namespace

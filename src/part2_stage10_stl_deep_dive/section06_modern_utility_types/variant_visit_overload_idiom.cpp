// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : variant_visit_overload_idiom
// Topic id : part2/stage10/section06/variant_visit_overload_idiom

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <variant>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/variant_visit_overload_idiom";

template <class... Callables>
struct Overload : Callables... {
    using Callables::operator()...;
};
template <class... Callables>
Overload(Callables...) -> Overload<Callables...>;

struct Connected {
    int peer;
};
struct Data {
    std::string text;
};
struct Error {
    int code;
};

using Event = std::variant<Connected, Data, Error>;

const auto describe = Overload{
    [](const Connected& value) { return "connected:" + std::to_string(value.peer); },
    [](const Data& value) { return "data:" + value.text; },
    [](const Error& value) { return "error:" + std::to_string(value.code); },
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    Event event = Connected{7};
    LEARN_EXPECT_EQ(checks, std::visit(describe, event), "connected:7");
    event = Data{"hello"};
    LEARN_EXPECT_EQ(checks, std::visit(describe, event), "data:hello");
    event = Error{404};
    LEARN_EXPECT_EQ(checks, std::visit(describe, event), "error:404");

    const std::variant<int, double> left = 3;
    const std::variant<int, double> right = 2.5;
    const auto add = [](const auto a, const auto b) { return static_cast<double>(a + b); };
    LEARN_EXPECT_EQ(checks, std::visit(add, left, right), 5.5);

    static_assert(std::is_invocable_r_v<std::string, decltype(describe), const Connected&>);
    static_assert(std::is_invocable_r_v<std::string, decltype(describe), const Data&>);
    static_assert(std::is_invocable_r_v<std::string, decltype(describe), const Error&>);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/variant_visit_overload_idiom", run>;

}  // namespace

// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : variant_visit_derived_cpp23
// Topic id : part2/stage10/section06/variant_visit_derived_cpp23

#ifdef __has_include
#if __has_include(<variant>)
#include <variant>
#define LEARN_HAS_VARIANT_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/variant_visit_derived_cpp23";

#if defined(LEARN_HAS_VARIANT_HEADER) && defined(__cpp_lib_variant) && __cpp_lib_variant >= 201606L

class CommandResult : public std::variant<int, std::string> {
public:
    using Base = std::variant<int, std::string>;
    using Base::Base;
    using Base::operator=;
};

const auto render = []<class Value>(const Value& value) {
    if constexpr (std::is_same_v<Value, int>) {
        return "code:" + std::to_string(value);
    } else {
        return "message:" + value;
    }
};

[[nodiscard]] std::string visit_compatibly(const CommandResult& result) {
    return std::visit(render, static_cast<const CommandResult::Base&>(result));
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_VARIANT_HEADER) && defined(__cpp_lib_variant) && __cpp_lib_variant >= 201606L
    learn::ExampleChecks checks{topic_id};
    CommandResult result = 200;
    LEARN_EXPECT_EQ(checks, visit_compatibly(result), "code:200");

#if __cpp_lib_variant >= 202102L
    // P2162 allows std::visit to accept a class publicly derived from std::variant.
    LEARN_EXPECT_EQ(checks, std::visit(render, result), "code:200");
    result = std::string{"ok"};
    LEARN_EXPECT_EQ(checks, std::visit(render, result), "message:ok");
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "__cpp_lib_variant >= 202102L (P2162)");
#endif
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 std::variant");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/variant_visit_derived_cpp23", run>;

}  // namespace

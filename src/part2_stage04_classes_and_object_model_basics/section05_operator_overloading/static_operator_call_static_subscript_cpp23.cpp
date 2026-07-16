// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : static_operator_call_static_subscript_cpp23
// Topic id : part2/stage04/section05/static_operator_call_static_subscript_cpp23
// References: [over.call], [over.sub], P1169R4, P2589R1

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section05/static_operator_call_static_subscript_cpp23";

struct CompileTimeTools {
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    static constexpr int operator()(int value) noexcept { return value * 3; }
    static constexpr int operator[](std::size_t index) noexcept {
        constexpr std::array values{2, 3, 5};
        return values[index];
    }
#else
    static constexpr int call(int value) noexcept { return value * 3; }
    static constexpr int index(std::size_t value) noexcept {
        constexpr std::array values{2, 3, 5};
        return values[value];
    }
#endif
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    static_assert(CompileTimeTools{}(4) == 12);
    static_assert(CompileTimeTools{}[2] == 5);
    LEARN_EXPECT_EQ(checks, CompileTimeTools::operator()(6), 18);
#else
    static_assert(CompileTimeTools::call(4) == 12);
    static_assert(CompileTimeTools::index(2) == 5);
    LEARN_EXPECT_EQ(checks, CompileTimeTools::call(6), 18);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage04/section05/static_operator_call_static_subscript_cpp23", run>;

}  // namespace

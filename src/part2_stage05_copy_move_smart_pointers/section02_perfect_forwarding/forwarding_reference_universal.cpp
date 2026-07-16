// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : forwarding_reference_universal
// Topic id : part2/stage05/section02/forwarding_reference_universal
// References: [temp.deduct.call], [dcl.ref]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section02/forwarding_reference_universal";

template <class T>
constexpr std::string_view deduced_category(T&&) noexcept {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return "lvalue";
    } else {
        return "rvalue";
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::string name = "named";
    LEARN_EXPECT_EQ(checks, deduced_category(name), std::string_view{"lvalue"});
    LEARN_EXPECT_EQ(checks, deduced_category(std::string{"temporary"}), std::string_view{"rvalue"});

    const std::string constant = "const";
    LEARN_EXPECT_EQ(checks, deduced_category(constant), std::string_view{"lvalue"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section02/forwarding_reference_universal", run>;

}  // namespace

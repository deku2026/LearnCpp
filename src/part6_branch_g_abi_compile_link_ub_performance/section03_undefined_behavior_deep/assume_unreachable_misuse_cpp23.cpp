// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : assume_unreachable_misuse_cpp23
// Topic id : part6/g/section03/assume_unreachable_misuse_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <optional>
#include <string_view>
#include <utility>
#include <version>

namespace {

enum class Command { start, stop, pause };

[[nodiscard]] constexpr std::optional<std::string_view> command_name(Command command) noexcept {
    switch (command) {
        case Command::start:
            return "start";
        case Command::stop:
            return "stop";
        case Command::pause:
            return "pause";
    }
    return std::nullopt;  // Defensive result for values arriving across an ABI boundary.
}

#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(assume) >= 202207L
constexpr bool standard_assume_available = true;
#else
constexpr bool standard_assume_available = false;
#endif
#else
constexpr bool standard_assume_available = false;
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/assume_unreachable_misuse_cpp23"};

    LEARN_EXPECT_EQ(checks, command_name(Command::start), std::optional<std::string_view>{"start"});
    LEARN_EXPECT_EQ(checks, command_name(Command::pause), std::optional<std::string_view>{"pause"});
    constexpr std::string_view assume_status =
        standard_assume_available ? "standard assume attribute available" : "checked branch retained";
    LEARN_EXPECT(checks, !assume_status.empty());

#if 0
    int value = external_input();
    [[assume(value > 0)]];  // If false at runtime, behavior is undefined.
#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
    std::unreachable();     // Reaching this call is itself undefined behavior.
#endif
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/assume_unreachable_misuse_cpp23", run>;

}  // namespace

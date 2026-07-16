// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : partial_ordering_of_function_templates
// Topic id : part6/d/section03/partial_ordering_of_function_templates
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section03/partial_ordering_of_function_templates";

enum class TemplateKind { value, pointer, pointer_to_const };

template <class T>
[[nodiscard]] constexpr TemplateKind kind(T) {
    return TemplateKind::value;
}

template <class T>
[[nodiscard]] constexpr TemplateKind kind(T*) {
    return TemplateKind::pointer;
}

template <class T>
[[nodiscard]] constexpr TemplateKind kind(const T*) {
    return TemplateKind::pointer_to_const;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int value{};
    const int constant{};
    LEARN_EXPECT(checks, kind(value) == TemplateKind::value);
    LEARN_EXPECT(checks, kind(&value) == TemplateKind::pointer);
    LEARN_EXPECT(checks, kind(&constant) == TemplateKind::pointer_to_const);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part6/d/section03/partial_ordering_of_function_templates", run>;

}  // namespace

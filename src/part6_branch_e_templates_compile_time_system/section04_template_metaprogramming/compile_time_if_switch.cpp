// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : compile_time_if_switch
// Topic id : part6/e/section04/compile_time_if_switch

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <string_view>
#include <type_traits>

namespace {

enum class WireKind { integer, floating, text, unsupported };

template <class T>
[[nodiscard]] consteval WireKind wire_kind() {
    using Value = std::remove_cvref_t<T>;
    if constexpr (std::is_integral_v<Value>) {
        return WireKind::integer;
    } else if constexpr (std::is_floating_point_v<Value>) {
        return WireKind::floating;
    } else if constexpr (std::is_same_v<Value, std::string_view>) {
        return WireKind::text;
    } else {
        return WireKind::unsupported;
    }
}

template <int Version>
[[nodiscard]] constexpr std::string_view protocol_name() noexcept {
    if constexpr (Version == 1) {
        return "legacy";
    } else if constexpr (Version == 2) {
        return "stable";
    } else if constexpr (Version == 3) {
        return "experimental";
    } else {
        return "unknown";
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section04/compile_time_if_switch"};

    static_assert(wire_kind<int>() == WireKind::integer);
    static_assert(wire_kind<double>() == WireKind::floating);
    static_assert(wire_kind<std::string_view>() == WireKind::text);
    static_assert(protocol_name<2>() == "stable");
    LEARN_EXPECT_EQ(checks, protocol_name<3>(), std::string_view{"experimental"});
    LEARN_EXPECT_EQ(checks, wire_kind<void*>(), WireKind::unsupported);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/compile_time_if_switch", run>;

}  // namespace

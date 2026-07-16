// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section01_instantiation_model
// Item     : two_phase_lookup_recap
// Topic id : part6/e/section01/two_phase_lookup_recap

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <concepts>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

template <class T>
[[nodiscard]] constexpr auto describe_later(const T& value) noexcept(noexcept(describe(value)))
    -> decltype(describe(value)) {
    // The call is dependent. Ordinary lookup is fixed at definition, while ADL
    // also examines associated namespaces when this specialization is formed.
    return describe(value);
}

namespace domain {
struct Token {
    int code;
};

[[nodiscard]] constexpr std::string_view describe(const Token& token) noexcept {
    return token.code == 23 ? "C++23 token" : "other token";
}
}  // namespace domain

template <class Container>
using element_t = typename Container::value_type;

struct Converter {
    template <class T>
    [[nodiscard]] constexpr T as() const {
        return static_cast<T>(42);
    }
};

template <class T>
[[nodiscard]] constexpr int dependent_member_template(const T& value) {
    return value.template as<int>();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section01/two_phase_lookup_recap"};

    static_assert(describe_later(domain::Token{23}) == "C++23 token");
    static_assert(std::same_as<element_t<std::vector<long>>, long>);
    static_assert(dependent_member_template(Converter{}) == 42);
    LEARN_EXPECT_EQ(checks, describe_later(domain::Token{7}), std::string_view{"other token"});
    LEARN_EXPECT_EQ(checks, dependent_member_template(Converter{}), 42);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section01/two_phase_lookup_recap", run>;

}  // namespace

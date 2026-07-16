// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section02_dependent_names
// Item     : two_phase_lookup
// Topic id : part6/d/section02/two_phase_lookup
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section02/two_phase_lookup";

[[nodiscard]] constexpr int fixed_policy() {
    return 10;
}

namespace extension {
struct Token {
    int value{};
};
}  // namespace extension

template <class T>
[[nodiscard]] constexpr int evaluate(const T& value) {
    // fixed_policy is non-dependent and is bound at template definition.
    // customize(value) is dependent and is completed by ADL at instantiation.
    return fixed_policy() + customize(value);
}

namespace extension {
[[nodiscard]] constexpr int customize(Token token) {
    return token.value * 2;
}
}  // namespace extension

template <class T>
struct Base {
    int value = 7;
};

template <class T>
struct Derived : Base<T> {
    [[nodiscard]] constexpr int read() const {
        // A member of a dependent base is not found by unqualified phase-one
        // lookup. this-> makes the member access dependent.
        return this->value;
    }
};

static_assert(evaluate(extension::Token{5}) == 20);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, evaluate(extension::Token{6}), 22);
    LEARN_EXPECT_EQ(checks, Derived<double>{}.read(), 7);

#if 0
    template <class T>
    struct Broken : Base<T> {
        int read() const { return value; }  // value is not found in phase one.
    };
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section02/two_phase_lookup", run>;

}  // namespace

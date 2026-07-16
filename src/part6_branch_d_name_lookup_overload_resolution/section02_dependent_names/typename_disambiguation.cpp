// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section02_dependent_names
// Item     : typename_disambiguation
// Topic id : part6/d/section02/typename_disambiguation
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <array>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/d/section02/typename_disambiguation";

template <class Container>
[[nodiscard]] constexpr typename Container::value_type first_or_default(const Container& values) {
    // Both names depend on Container and denote types, so typename is required.
    using value_type = typename Container::value_type;
    typename Container::const_iterator position = values.begin();
    return position == values.end() ? value_type{} : *position;
}

template <class T>
struct Identity {
    using type = T;
};

template <class T>
using identity_t = typename Identity<T>::type;

static_assert(std::is_same_v<identity_t<long>, long>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const std::vector<int> populated{9, 8};
    const std::vector<int> empty;
    constexpr std::array values{4, 5, 6};
    LEARN_EXPECT_EQ(checks, first_or_default(populated), 9);
    LEARN_EXPECT_EQ(checks, first_or_default(empty), 0);
    LEARN_EXPECT_EQ(checks, first_or_default(values), 4);

#if 0
    template <class T>
    void ambiguous_parse() {
        T::value_type* pointer;  // May parse as multiplication, not a declaration.
    }
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section02/typename_disambiguation", run>;

}  // namespace

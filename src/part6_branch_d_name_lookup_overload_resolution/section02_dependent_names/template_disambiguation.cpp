// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section02_dependent_names
// Item     : template_disambiguation
// Topic id : part6/d/section02/template_disambiguation
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/d/section02/template_disambiguation";

struct Converter {
    int value{};

    template <class T>
    [[nodiscard]] constexpr T as() const {
        return static_cast<T>(value);
    }
};

template <class Object>
[[nodiscard]] constexpr int convert_to_int(const Object& object) {
    // After a dependent . or ->, template says that as names a member template;
    // otherwise '<' could begin a less-than expression.
    return object.template as<int>();
}

template <class T>
struct AllocatorModel {
    template <class U>
    struct rebind {
        using type = U;
    };
};

template <class Allocator, class U>
using rebound_t = typename Allocator::template rebind<U>::type;

static_assert(std::is_same_v<rebound_t<AllocatorModel<int>, double>, double>);
static_assert(convert_to_int(Converter{42}) == 42);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, convert_to_int(Converter{17}), 17);
    LEARN_EXPECT(checks, (std::is_same_v<rebound_t<AllocatorModel<int>, long>, long>));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section02/template_disambiguation", run>;

}  // namespace

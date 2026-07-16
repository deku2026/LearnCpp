// void_t turns well-formed dependent types and expressions into a detection signal.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : void_t
// Topic id : part2/stage06/section04/void_t
//
// Failed substitutions fall back to each false primary template instead of becoming errors.

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

template <class T, class = void>
struct HasValueType : std::false_type {};

template <class T>
struct HasValueType<T, std::void_t<typename T::value_type>> : std::true_type {};

template <class T, class = void>
struct HasSize : std::false_type {};

template <class T>
struct HasSize<T, std::void_t<decltype(std::declval<const T&>().size())>> : std::true_type {};

static_assert(HasValueType<std::vector<int>>::value);
static_assert(!HasValueType<int>::value);
static_assert(HasSize<std::string>::value);
static_assert(!HasSize<double>::value);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section04/void_t"};
    LEARN_EXPECT(checks, HasValueType<std::string>::value);
    LEARN_EXPECT(checks, HasSize<std::vector<int>>::value);
    LEARN_EXPECT(checks, !HasSize<int>::value);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/void_t", run>;

}  // namespace

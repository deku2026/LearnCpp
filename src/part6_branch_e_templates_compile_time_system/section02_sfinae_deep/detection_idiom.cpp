// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section02_sfinae_deep
// Item     : detection_idiom
// Topic id : part6/e/section02/detection_idiom

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

struct nonesuch {
    nonesuch() = delete;
    ~nonesuch() = delete;
    nonesuch(const nonesuch&) = delete;
    void operator=(const nonesuch&) = delete;
};

template <class Default, class AlwaysVoid, template <class...> class Operation, class... Arguments>
struct detector {
    using value_t = std::false_type;
    using type = Default;
};

template <class Default, template <class...> class Operation, class... Arguments>
struct detector<Default, std::void_t<Operation<Arguments...>>, Operation, Arguments...> {
    using value_t = std::true_type;
    using type = Operation<Arguments...>;
};

template <template <class...> class Operation, class... Arguments>
inline constexpr bool is_detected_v = detector<nonesuch, void, Operation, Arguments...>::value_t::value;

template <template <class...> class Operation, class... Arguments>
using detected_t = typename detector<nonesuch, void, Operation, Arguments...>::type;

template <class T>
using size_expression_t = decltype(std::declval<const T&>().size());

template <class T>
using append_expression_t = decltype(std::declval<T&>().push_back(std::declval<typename T::value_type>()));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section02/detection_idiom"};

    static_assert(is_detected_v<size_expression_t, std::string>);
    static_assert(is_detected_v<append_expression_t, std::vector<int>>);
    static_assert(!is_detected_v<size_expression_t, int>);
    static_assert(std::is_same_v<detected_t<size_expression_t, std::vector<int>>, std::vector<int>::size_type>);
    LEARN_EXPECT(checks, (is_detected_v<size_expression_t, std::vector<double>>));
    LEARN_EXPECT(checks, !(is_detected_v<append_expression_t, int>));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section02/detection_idiom", run>;

}  // namespace

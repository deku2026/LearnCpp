// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : type_lists
// Topic id : part6/e/section04/type_lists

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <type_traits>

namespace {

template <class... Types>
struct type_list {};

template <class List>
struct list_size;

template <class... Types>
struct list_size<type_list<Types...>> : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <class List>
struct front;

template <class First, class... Rest>
struct front<type_list<First, Rest...>> {
    using type = First;
};

template <class List, class New>
struct push_back;

template <class... Types, class New>
struct push_back<type_list<Types...>, New> {
    using type = type_list<Types..., New>;
};

template <class List, class Query>
struct contains;

template <class Query, class... Types>
struct contains<type_list<Types...>, Query> : std::bool_constant<(std::is_same_v<Query, Types> || ...)> {};

template <class List, template <class> class Transform>
struct transform;

template <template <class> class Transform, class... Types>
struct transform<type_list<Types...>, Transform> {
    using type = type_list<typename Transform<Types>::type...>;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section04/type_lists"};

    using source = type_list<int, double, char>;
    using extended = typename push_back<source, long>::type;
    using pointers = typename transform<source, std::add_pointer>::type;
    static_assert(list_size<source>::value == 3);
    static_assert(std::is_same_v<typename front<source>::type, int>);
    static_assert(list_size<extended>::value == 4);
    static_assert(contains<source, double>::value);
    static_assert(!contains<source, void>::value);
    static_assert(std::is_same_v<pointers, type_list<int*, double*, char*>>);
    LEARN_EXPECT_EQ(checks, list_size<extended>::value, 4U);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/type_lists", run>;

}  // namespace

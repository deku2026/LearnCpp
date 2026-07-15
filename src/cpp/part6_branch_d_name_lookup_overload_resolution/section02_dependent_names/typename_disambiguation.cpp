// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D4 typename)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section02_dependent_names
// Item     : typename_disambiguation
// Topic id : part6/d/section02/typename_disambiguation
//
// 要点: 依赖名默认当非类型；是类型时必须 typename。
// 参考: [temp.res]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <type_traits>
#include <vector>

namespace {

template <typename C>
typename C::value_type first_value(const C& c) {
    typename C::const_iterator it = c.begin();
    return *it;
}

template <typename T>
struct Holder {
    using nested = T;
};

template <typename T>
typename Holder<T>::nested make_one(T x) {
    return x;
}

// 进阶: 嵌套依赖类型
template <typename Map>
typename Map::mapped_type get_zero(const Map& m) {
    typename Map::const_iterator it = m.find(0);
    assert(it != m.end());
    return it->second;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D4 typename disambiguation ===\n";

    std::vector<int> v{10, 20, 30};
    assert(first_value(v) == 10);
    assert(make_one(5) == 5);

    static_assert(std::is_same_v<typename Holder<int>::nested, int>);

    std::map<int, std::string> m{{0, "ok"}, {1, "x"}};
    assert(get_zero(m) == "ok");

    std::cout << "  without typename: T::type * p may parse as multiply\n";
    std::cout << "  C++20 often allows omitting typename in more contexts\n";
    std::cout << "typename_disambiguation: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section02/typename_disambiguation", run>;

}  // namespace

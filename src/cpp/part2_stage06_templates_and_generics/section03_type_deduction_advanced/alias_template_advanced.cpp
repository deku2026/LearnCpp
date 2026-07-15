// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : alias_template_advanced
// Topic id : part2/stage06/section03/alias_template_advanced
//
// Covers: alias templates, metafunction shortcuts, nested alias patterns

#include "learn/topic_registry.hpp"

#include <cassert>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

namespace {

template <typename T>
using Vec = std::vector<T>;

template <typename K, typename V = int>
using Dict = std::map<K, V>;

template <typename T>
struct Identity {
    using type = T;
};

template <typename T>
using Identity_t = typename Identity<T>::type;

template <typename T>
using RemoveConstRef = std::remove_const_t<std::remove_reference_t<T>>;

void demo_basics() {
    Vec<int> v{1, 2, 3};
    assert(v.size() == 3);
    static_assert(std::is_same_v<Vec<int>, std::vector<int>>);
}

void demo_intermediate() {
    Dict<std::string> d;
    d["a"] = 1;
    assert(d["a"] == 1);
    static_assert(std::is_same_v<Dict<std::string>, std::map<std::string, int>>);

    Dict<int, std::string> d2;
    d2[1] = "one";
    assert(d2[1] == "one");
}

void demo_expert() {
    static_assert(std::is_same_v<Identity_t<double>, double>);
    static_assert(std::is_same_v<RemoveConstRef<const int&>, int>);
    static_assert(std::is_same_v<RemoveConstRef<int&&>, int>);

    using VI = Vec<Identity_t<int>>;
    VI xs{7};
    assert(xs[0] == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/alias_template_advanced", run>;

}  // namespace

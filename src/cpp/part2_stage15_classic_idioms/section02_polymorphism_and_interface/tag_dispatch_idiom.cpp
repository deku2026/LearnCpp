// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §2.4
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : tag_dispatch_idiom
// Topic id : part2/stage15/section02/tag_dispatch_idiom
//
// Tag dispatch: empty tag types select overloads at compile time.
// Motive : specialize algorithms by traits (iterator category, etc.) without runtime if.
// Modern : Concepts + if constexpr cover many cases more readably; tags still appear
//          in older libraries and when overload sets need fine control.
// Pitfall: tag hierarchies must match inheritance of category tags carefully.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <iostream>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

namespace {

// ---------- classic iterator-category tags ----------
template <typename It>
void advance_impl(It& it, int n, std::random_access_iterator_tag) {
    std::cout << "  random_access path O(1)\n";
    it += n;
}

template <typename It>
void advance_impl(It& it, int n, std::bidirectional_iterator_tag) {
    std::cout << "  bidirectional path O(n)\n";
    if (n >= 0) {
        while (n--) {
            ++it;
        }
    } else {
        while (n++) {
            --it;
        }
    }
}

template <typename It>
void advance_impl(It& it, int n, std::input_iterator_tag) {
    std::cout << "  input path O(n)\n";
    while (n-- > 0) {
        ++it;
    }
}

template <typename It>
void my_advance(It& it, int n) {
    using Cat = typename std::iterator_traits<It>::iterator_category;
    advance_impl(it, n, Cat{});
}

// ---------- custom tag dispatch ----------
struct cpu_tag {};
struct gpu_tag {};

template <typename ExecTag>
struct executor_traits;

template <>
struct executor_traits<cpu_tag> {
    static constexpr const char* name = "cpu";
};

template <>
struct executor_traits<gpu_tag> {
    static constexpr const char* name = "gpu";
};

void launch_impl(cpu_tag) {
    std::cout << "  launch on CPU threads\n";
}
void launch_impl(gpu_tag) {
    std::cout << "  launch on GPU kernels\n";
}

template <typename Tag>
void launch(Tag t) {
    std::cout << "  executor=" << executor_traits<Tag>::name << '\n';
    launch_impl(t);
}

// ---------- modern replacements ----------
template <std::random_access_iterator It>
void modern_advance(It& it, int n) {
    std::cout << "  concept-constrained O(1)\n";
    it += n;
}

template <std::input_iterator It>
    requires(!std::random_access_iterator<It>)
void modern_advance(It& it, int n) {
    std::cout << "  concept-constrained O(n)\n";
    while (n-- > 0) {
        ++it;
    }
}

template <typename T>
void process(const T& x) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "  if constexpr integral " << x << '\n';
    } else {
        std::cout << "  if constexpr other\n";
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== tag dispatch by iterator_category ==\n";
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    auto vit = v.begin();
    my_advance(vit, 3);
    assert(*vit == 3);

    std::list<int> lst{10, 20, 30, 40};
    auto lit = lst.begin();
    my_advance(lit, 2);
    assert(*lit == 30);

    std::cout << "== custom empty tags ==\n";
    launch(cpu_tag{});
    launch(gpu_tag{});

    std::cout << "== modern: concepts / if constexpr ==\n";
    vit = v.begin();
    modern_advance(vit, 2);
    assert(*vit == 2);
    lit = lst.begin();
    modern_advance(lit, 1);
    assert(*lit == 20);
    process(42);
    process(3.14);

    std::cout << "new code: prefer concepts/if constexpr; know tags to read classic STL\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section02/tag_dispatch_idiom", run>;

}  // namespace

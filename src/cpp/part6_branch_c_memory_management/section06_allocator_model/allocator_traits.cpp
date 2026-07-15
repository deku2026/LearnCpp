// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 allocator_traits)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_traits
// Topic id : part6/c/section06/allocator_traits
//
// 要点: 容器经 allocator_traits 间接使用分配器；缺省提供 rebind/construct 等。
// 参考: https://en.cppreference.com/w/cpp/memory/allocator_traits

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

namespace {

template <typename T>
struct Minimal {
    using value_type = T;
    using is_always_equal = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    Minimal() = default;
    template <typename U>
    Minimal(const Minimal<U>&) noexcept {}

    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }

    template <typename U>
    struct rebind {
        using other = Minimal<U>;
    };

    friend bool operator==(const Minimal&, const Minimal&) { return true; }
    friend bool operator!=(const Minimal&, const Minimal&) { return false; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 allocator_traits ===\n";

    using AT = std::allocator_traits<Minimal<int>>;
    static_assert(std::is_same_v<AT::value_type, int>);
    static_assert(std::is_same_v<AT::pointer, int*>);

    Minimal<int> m;
    int* p = AT::allocate(m, 4);
    for (int i = 0; i < 4; ++i) AT::construct(m, p + i, i + 1);
    assert(p[0] == 1 && p[3] == 4);
    for (int i = 0; i < 4; ++i) AT::destroy(m, p + i);
    AT::deallocate(m, p, 4);

    // rebind via traits
    using Rebound = AT::rebind_alloc<double>;
    static_assert(std::is_same_v<std::allocator_traits<Rebound>::value_type, double>);

    std::vector<int, Minimal<int>> v;
    v.push_back(9);
    assert(v.front() == 9);

    std::cout << "  write allocate/deallocate; traits fill the rest\n";
    std::cout << "allocator_traits: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_traits", run>;

}  // namespace

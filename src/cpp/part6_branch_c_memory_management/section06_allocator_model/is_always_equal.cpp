// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 is_always_equal)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : is_always_equal
// Topic id : part6/c/section06/is_always_equal
//
// 要点: is_always_equal 表示任意两实例可互相 deallocate；无状态分配器默认真。
// 参考: allocator_traits::is_always_equal

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <type_traits>

namespace {

template <typename T>
struct Stateless {
    using value_type = T;
    Stateless() = default;
    template <typename U>
    Stateless(const Stateless<U>&) noexcept {}
    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }
    friend bool operator==(const Stateless&, const Stateless&) noexcept { return true; }
};

template <typename T>
struct Stateful {
    using value_type = T;
    int id = 0;
    using is_always_equal = std::false_type;
    Stateful() = default;
    explicit Stateful(int i) : id(i) {}
    template <typename U>
    Stateful(const Stateful<U>& o) noexcept : id(o.id) {}
    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }
    friend bool operator==(const Stateful& a, const Stateful& b) noexcept { return a.id == b.id; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 is_always_equal ===\n";

    static_assert(std::allocator_traits<std::allocator<int>>::is_always_equal::value);
    static_assert(std::allocator_traits<Stateless<int>>::is_always_equal::value);
    static_assert(!std::allocator_traits<Stateful<int>>::is_always_equal::value);

    Stateful<int> a{1}, b{1}, c{2};
    assert(a == b);
    assert(!(a == c));

    std::cout << "  always_equal => any instance may free any other allocation\n";
    std::cout << "is_always_equal: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/is_always_equal", run>;

}  // namespace

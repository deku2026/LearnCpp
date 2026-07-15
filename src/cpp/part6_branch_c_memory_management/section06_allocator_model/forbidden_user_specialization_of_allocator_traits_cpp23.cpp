// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 P2652)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : forbidden_user_specialization_of_allocator_traits_cpp23
// Topic id : part6/c/section06/forbidden_user_specialization_of_allocator_traits_cpp23
//
// 要点: C++23 禁止用户特化 allocator_traits；定制写在分配器成员里。
// 参考: P2652

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <new>
#include <utility>

namespace {

// ✅ 正确定制：在分配器上提供成员
template <typename T>
struct VerboseAlloc {
    using value_type = T;
    static inline int constructs = 0;

    VerboseAlloc() = default;
    template <typename U>
    VerboseAlloc(const VerboseAlloc<U>&) noexcept {}

    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        ++constructs;
        ::new (static_cast<void*>(p)) U(std::forward<Args>(args)...);
    }

    friend bool operator==(const VerboseAlloc&, const VerboseAlloc&) noexcept { return true; }
};

// ❌ 禁止：template<> struct std::allocator_traits<VerboseAlloc<int>> { ... };

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 P2652: do not specialize allocator_traits ===\n";

    VerboseAlloc<int>::constructs = 0;
    using AT = std::allocator_traits<VerboseAlloc<int>>;
    VerboseAlloc<int> a;
    int* p = AT::allocate(a, 1);
    AT::construct(a, p, 42);  // 走分配器 construct 成员
    assert(*p == 42);
    assert(VerboseAlloc<int>::constructs == 1);
    AT::destroy(a, p);
    AT::deallocate(a, p, 1);

    std::cout << "  customize via allocator members; traits detects them\n";
    std::cout << "  [layer] traits/pmr/pools: see branch C doc C6-C8 acceptance\n";
    std::cout << "forbidden_user_specialization_of_allocator_traits_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/c/section06/forbidden_user_specialization_of_allocator_traits_cpp23", run>;

}  // namespace

// Topic    : std::allocator + allocator_traits 基础
// Doc      : 第3部分-标准库系统化.md · 库 6.2
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : std_allocator
// Topic id : part3/section06/std_allocator
// Refs     : https://en.cppreference.com/w/cpp/memory/allocator
//            https://en.cppreference.com/w/cpp/memory/allocator_traits

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_allocator] 直接使用 allocator ===\n";
    {
        std::allocator<int> alloc;
        int* p = alloc.allocate(3);
        // C++20 起 construct/destroy 走 allocator_traits
        std::allocator_traits<std::allocator<int>>::construct(alloc, p, 1);
        std::allocator_traits<std::allocator<int>>::construct(alloc, p + 1, 2);
        std::allocator_traits<std::allocator<int>>::construct(alloc, p + 2, 3);
        assert(p[0] == 1 && p[1] == 2 && p[2] == 3);
        std::cout << "raw alloc: " << p[0] << ' ' << p[1] << ' ' << p[2] << '\n';
        for (int i = 0; i < 3; ++i) {
            std::allocator_traits<std::allocator<int>>::destroy(alloc, p + i);
        }
        alloc.deallocate(p, 3);
    }

    std::cout << "=== 容器默认模板参数即 allocator ===\n";
    {
        static_assert(std::is_same_v<std::vector<int>::allocator_type, std::allocator<int>>);
        std::vector<int> v{1, 2, 3};
        assert(v.get_allocator() == std::allocator<int>{});
        std::cout << "vector::allocator_type is std::allocator<int>\n";
    }

    std::cout << "=== allocator_traits 统一接口 ===\n";
    {
        using A = std::allocator<std::string>;
        using T = std::allocator_traits<A>;
        A a;
        auto* p = T::allocate(a, 1);
        T::construct(a, p, "traits");
        assert(*p == "traits");
        std::cout << "traits construct: " << *p << '\n';
        T::destroy(a, p);
        T::deallocate(a, p, 1);

        std::cout << "is_always_equal=" << std::boolalpha << T::is_always_equal::value << '\n';
    }

    std::cout << "note: rarely use allocator directly; prefer pmr for strategy\n";
    std::cout << "[std_allocator] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/std_allocator", run>;

}  // namespace

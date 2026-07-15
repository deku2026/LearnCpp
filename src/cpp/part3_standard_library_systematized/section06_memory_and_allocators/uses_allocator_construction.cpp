// Topic    : uses-allocator 构造与 scoped_allocator_adaptor
// Doc      : 第3部分-标准库系统化.md · 库 6.4
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : uses_allocator_construction
// Topic id : part3/section06/uses_allocator_construction
// Refs     : https://en.cppreference.com/w/cpp/memory/uses_allocator
//            https://en.cppreference.com/w/cpp/memory/scoped_allocator_adaptor

#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <scoped_allocator>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// 可感知分配器的类型
template <class Alloc = std::allocator<char>>
struct Tracked {
    using allocator_type = Alloc;

    std::basic_string<char, std::char_traits<char>, Alloc> text;
    int tag{};

    Tracked(int t, const Alloc& a = Alloc{}) : text{"tracked", a}, tag{t} {}
    Tracked(std::allocator_arg_t, const Alloc& a, int t) : text{"tracked", a}, tag{t} {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [uses_allocator_construction] uses_allocator trait ===\n";
    {
        using T = Tracked<>;
        static_assert(std::uses_allocator_v<T, std::allocator<char>>);
        std::cout << "uses_allocator_v<Tracked, allocator> = true\n";
    }

    std::cout << "=== make_obj_using_allocator ===\n";
    {
        std::allocator<char> a;
        auto obj = std::make_obj_using_allocator<Tracked<>>(a, 7);
        assert(obj.tag == 7);
        assert(obj.text == "tracked");
        std::cout << "make_obj_using_allocator tag=" << obj.tag << '\n';
    }

    std::cout << "=== pmr 传播：vector 与内嵌 string 同 resource ===\n";
    {
        std::array<std::byte, 1024> buffer{};
        std::pmr::monotonic_buffer_resource pool{buffer.data(), buffer.size()};

        std::pmr::vector<std::pmr::string> v{&pool};
        v.emplace_back("alpha");
        v.emplace_back("beta");
        assert(v[0] == "alpha");
        // polymorphic_allocator::construct 自动 uses-allocator 传播
        std::cout << "pmr nested strings: " << v[0] << ',' << v[1] << '\n';
    }

    std::cout << "=== scoped_allocator_adaptor（非 pmr 传播） ===\n";
    {
        using Inner = std::allocator<char>;
        using Outer = std::scoped_allocator_adaptor<std::allocator<std::string>, Inner>;
        // 简化演示：adaptor 存在且可默认构造
        Outer outer;
        std::vector<std::string, Outer> v{outer};
        v.emplace_back("scoped");
        assert(v[0] == "scoped");
        std::cout << "scoped_allocator_adaptor vector: " << v[0] << '\n';
    }

    std::cout << "pmr preferred for runtime strategy; scoped_allocator for static nesting\n";
    std::cout << "[uses_allocator_construction] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/uses_allocator_construction", run>;

}  // namespace

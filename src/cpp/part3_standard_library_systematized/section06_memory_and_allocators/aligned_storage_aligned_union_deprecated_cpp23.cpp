// Topic    : aligned_storage / aligned_union 在 C++23 弃用；用 alignas + byte[]
// Doc      : 第3部分-标准库系统化.md · 库 6.4
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : aligned_storage_aligned_union_deprecated_cpp23
// Topic id : part3/section06/aligned_storage_aligned_union_deprecated_cpp23
// Refs     : https://en.cppreference.com/w/cpp/types/aligned_storage
//            P1413；prefer alignas + std::byte

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <new>
#include <type_traits>

namespace {

struct alignas(16) SimdLike {
    float v[4];
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [aligned_storage_aligned_union_deprecated_cpp23] ===\n";
    std::cout << "C++23: std::aligned_storage / aligned_union are deprecated\n";
    std::cout << "replacement: alignas(T) std::byte storage[sizeof(T)];\n";

    // 现代写法：手动对齐存储 + placement new
    {
        alignas(SimdLike) std::byte storage[sizeof(SimdLike)]{};
        auto* p = ::new (storage) SimdLike{{1.f, 2.f, 3.f, 4.f}};
        assert(p->v[0] == 1.f && p->v[3] == 4.f);
        const auto addr = reinterpret_cast<std::uintptr_t>(p);
        assert(addr % alignof(SimdLike) == 0);
        std::cout << "SimdLike align=" << alignof(SimdLike) << " addr%align=" << (addr % alignof(SimdLike))
                  << " v3=" << p->v[3] << '\n';
        p->~SimdLike();
    }

    // 联合体式存储：用 alignas(max) + 足够大的 byte 缓冲
    {
        constexpr std::size_t cap = sizeof(double) > sizeof(int) ? sizeof(double) : sizeof(int);
        constexpr std::size_t al = alignof(double) > alignof(int) ? alignof(double) : alignof(int);
        alignas(al) std::byte storage[cap]{};
        auto* d = ::new (storage) double(3.25);
        assert(*d == 3.25);
        std::destroy_at(d);
        auto* i = ::new (storage) int(99);
        assert(*i == 99);
        std::destroy_at(i);
        std::cout << "manual aligned union-like storage ok\n";
    }

#if defined(__GNUC__) || defined(_MSC_VER)
    // 若实现仍提供旧别名，仅探测类型存在（可能带 deprecation 警告）
    // 教学代码避免实际使用 deprecated 类型，防止 -Werror
    std::cout << "do not use aligned_storage_t in new code\n";
#endif

    std::cout << "also see: operator new(size, align_val_t), std::aligned_alloc\n";
    std::cout << "[aligned_storage_aligned_union_deprecated_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/aligned_storage_aligned_union_deprecated_cpp23", run>;

}  // namespace

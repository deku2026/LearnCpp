// Topic    : headers map · 内存 memory
// Doc      : 第3部分-标准库系统化.md · headers map + 库6
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : memory_headers
// Topic id : part3/section08/memory_headers
// Headers  : <memory> <memory_resource> <new>
//
// 要点: 智能指针 / allocator 设施 / pmr / new_handler / 对齐分配 地图探针。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <new>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [memory_headers] map probe ===\n";
    std::cout << "<memory>          unique/shared/weak, allocator, out_ptr, start_lifetime_as\n";
    std::cout << "<memory_resource> pmr polymorphic resources (C++17)\n";
    std::cout << "<new>             new_handler, align_val_t, launder, destroying_delete\n";

    // --- <memory>: 所有权三件套 ---
    auto up = std::make_unique<int>(1);
    auto sp = std::make_shared<int>(2);
    std::weak_ptr<int> wp = sp;
    assert(*up == 1 && *sp == 2 && sp.use_count() == 1);
    assert(!wp.expired());
    {
        auto locked = wp.lock();
        assert(locked && *locked == 2);
    }
    std::cout << "unique/shared/weak ok use_count=" << sp.use_count() << '\n';

    // allocator_traits 最小面
    using A = std::allocator<int>;
    A a;
    int* raw_a = std::allocator_traits<A>::allocate(a, 2);
    std::allocator_traits<A>::construct(a, raw_a, 10);
    std::allocator_traits<A>::construct(a, raw_a + 1, 20);
    assert(raw_a[0] == 10 && raw_a[1] == 20);
    std::allocator_traits<A>::destroy(a, raw_a + 1);
    std::allocator_traits<A>::destroy(a, raw_a);
    std::allocator_traits<A>::deallocate(a, raw_a, 2);

    // --- <memory_resource>: pmr 缓冲 ---
    std::array<std::byte, 512> buf{};
    std::pmr::monotonic_buffer_resource pool{buf.data(), buf.size()};
    std::pmr::vector<int> v{&pool};
    v.push_back(42);
    v.push_back(43);
    assert(v.size() == 2 && v.front() == 42);
    std::cout << "pmr::vector on monotonic buffer size=" << v.size() << '\n';

    // --- <new>: handler + aligned new ---
    std::new_handler handler = std::get_new_handler();
    std::cout << "get_new_handler ptr=" << reinterpret_cast<const void*>(handler) << '\n';

    void* raw = ::operator new(16, std::align_val_t{16});
    assert(raw != nullptr);
    assert(reinterpret_cast<std::uintptr_t>(raw) % 16 == 0);
    ::operator delete(raw, std::align_val_t{16});
    std::cout << "aligned operator new/delete ok\n";

    // launder 平凡可用性(深层见 section06/std_launder)
    [[maybe_unused]] int x = 7;
    assert(*std::launder(&x) == 7);

    std::cout << "[memory_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/memory_headers", run>;

}  // namespace

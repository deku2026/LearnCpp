// Topic    : allocator_traits::allocate_at_least（C++23）
// Doc      : 第3部分-标准库系统化.md · 库 6.2
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : allocate_at_least_cpp23
// Topic id : part3/section06/allocate_at_least_cpp23
// Refs     : https://en.cppreference.com/w/cpp/memory/allocator_traits/allocate_at_least
//            P0401；__cpp_lib_allocate_at_least

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [allocate_at_least_cpp23] ===\n";

#if defined(__cpp_lib_allocate_at_least) && __cpp_lib_allocate_at_least >= 202302L
    std::cout << "__cpp_lib_allocate_at_least=" << __cpp_lib_allocate_at_least << '\n';

    std::allocator<int> alloc;
    // 请求至少 4 个；实现可能返回更多（减少后续扩容）
    auto result = std::allocator_traits<std::allocator<int>>::allocate_at_least(alloc, 4);
    assert(result.ptr != nullptr);
    assert(result.count >= 4);
    std::cout << "requested=4 actual_count=" << result.count << '\n';

    for (std::size_t i = 0; i < 4; ++i) {
        std::allocator_traits<std::allocator<int>>::construct(alloc, result.ptr + i, static_cast<int>(i + 1));
    }
    assert(result.ptr[0] == 1 && result.ptr[3] == 4);

    for (std::size_t i = 0; i < 4; ++i) {
        std::allocator_traits<std::allocator<int>>::destroy(alloc, result.ptr + i);
    }
    // deallocate 必须用实际 count（或实现允许的匹配分配量）
    alloc.deallocate(result.ptr, result.count);

    std::cout << "motivation: vector can adopt extra capacity from allocator\n";
#else
    std::cout << "allocate_at_least not available; classic allocate path\n";
    std::allocator<int> alloc;
    int* p = alloc.allocate(4);
    for (int i = 0; i < 4; ++i) {
        std::allocator_traits<std::allocator<int>>::construct(alloc, p + i, i + 1);
    }
    assert(p[0] == 1 && p[3] == 4);
    for (int i = 0; i < 4; ++i) {
        std::allocator_traits<std::allocator<int>>::destroy(alloc, p + i);
    }
    alloc.deallocate(p, 4);
    std::cout << "classic allocate(4) ok\n";
#endif

    // vector 容量探测（与 at_least 同一性能故事）
    std::vector<int> v;
    v.reserve(4);
    std::cout << "vector reserve(4) capacity=" << v.capacity() << '\n';

    std::cout << "[allocate_at_least_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/allocate_at_least_cpp23", run>;

}  // namespace

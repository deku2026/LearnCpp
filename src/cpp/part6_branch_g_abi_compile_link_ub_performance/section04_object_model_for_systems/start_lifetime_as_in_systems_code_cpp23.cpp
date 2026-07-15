// LearnCpp topic
// Doc      : 第6部分-支线G · G9.3 start_lifetime_as
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : start_lifetime_as_in_systems_code_cpp23
// Topic id : part6/g/section04/start_lifetime_as_in_systems_code_cpp23
//
// 要点: std::start_lifetime_as 在已有存储上开始 T 的生命周期 (C++23);
//       用于序列化/网络缓冲/共享内存。库缺失时用 placement new 等价演示。
// 参考: P2590

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <new>
#include <type_traits>

#if defined(__cpp_lib_start_lifetime_as)
#include <memory>
#endif

namespace {

struct Header {
    std::uint32_t magic;
    std::uint32_t length;
};

// 可移植: 在对齐缓冲上开始 Header 生命周期
Header* begin_header(void* storage, std::size_t n) {
    assert(n >= sizeof(Header));
#if defined(__cpp_lib_start_lifetime_as)
    return std::start_lifetime_as<Header>(storage);
#else
    // 教学后备: placement new 默认初始化平凡类型
    return new (storage) Header{};
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G9 start_lifetime_as (systems code) ===\n";

    alignas(Header) unsigned char wire[sizeof(Header)]{};
    // 模拟从网络填入对象表示
    Header pattern{0xDEADBEEFu, 16};
    std::memcpy(wire, &pattern, sizeof pattern);

    Header* h = begin_header(wire, sizeof wire);
    // 读出 — 对平凡类型, memcpy 进存储后 start_lifetime_as 合法化访问
    // 后备 placement 已写零, 重新 memcpy
    std::memcpy(h, &pattern, sizeof pattern);
    assert(h->magic == 0xDEADBEEFu);
    assert(h->length == 16);

    // 数组形式: start_lifetime_as_array (有库时)
    alignas(int) unsigned char ints[sizeof(int) * 3]{};
    int vals[3]{1, 2, 3};
    std::memcpy(ints, vals, sizeof vals);
#if defined(__cpp_lib_start_lifetime_as)
    int* arr = std::start_lifetime_as_array<int>(ints, 3);
#else
    int* arr = new (ints) int[3];
    std::memcpy(arr, vals, sizeof vals);
#endif
    assert(arr[0] == 1 && arr[2] == 3);

#if defined(__cpp_lib_start_lifetime_as)
    std::cout << "  std::start_lifetime_as available\n";
#else
    std::cout << "  fallback placement new (stdlib may lag C++23)\n";
#endif
    std::cout << "  use for mmap/network buffers of implicit-lifetime T\n";
    std::cout << "start_lifetime_as_in_systems_code_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/start_lifetime_as_in_systems_code_cpp23", run>;

}  // namespace

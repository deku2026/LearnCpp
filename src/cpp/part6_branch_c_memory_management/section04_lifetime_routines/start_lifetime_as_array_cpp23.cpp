// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C5 start_lifetime_as 数组)
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : start_lifetime_as_array_cpp23
// Topic id : part6/c/section04/start_lifetime_as_array_cpp23
//
// 要点: start_lifetime_as_array 在原始字节上开始 T[n] 生命周期。
// 参考: std::start_lifetime_as_array

#include "learn/topic_registry.hpp"

#include <cstring>
#include <iostream>
#include <memory>
#include <new>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C5 start_lifetime_as_array (C++23) ===\n";

    constexpr std::size_t n = 4;
    alignas(int) unsigned char buf[sizeof(int) * n];
    int src[n] = {10, 20, 30, 40};
    std::memcpy(buf, src, sizeof(src));

#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L
    int* arr = std::start_lifetime_as_array<int>(buf, n);
    assert(arr[0] == 10);
    assert(arr[3] == 40);
    arr[1] = 99;
    assert(arr[1] == 99);
    std::cout << "  array form: n elements, bytes preserved\n";
#else
    [[maybe_unused]] auto* arr = std::launder(reinterpret_cast<int*>(buf));
    assert(arr[0] == 10 && arr[3] == 40);
    std::cout << "  start_lifetime_as_array unavailable; launder demo\n";
#endif

    std::cout << "  use case: deserialize POD arrays from network/mmap buffers\n";
    std::cout << "start_lifetime_as_array_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/start_lifetime_as_array_cpp23", run>;

}  // namespace

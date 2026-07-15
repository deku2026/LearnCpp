// Topic    : pmr::monotonic_buffer_resource（栈缓冲极速 arena）
// Doc      : 第3部分-标准库系统化.md · 库 6.3
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : monotonic_buffer_resource_cpp17
// Topic id : part3/section06/monotonic_buffer_resource_cpp17
// Refs     : https://en.cppreference.com/w/cpp/memory/monotonic_buffer_resource

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [monotonic_buffer_resource_cpp17] 栈 buffer + pmr::vector ===\n";

    std::array<std::byte, 2048> buffer{};
    std::pmr::monotonic_buffer_resource pool{buffer.data(), buffer.size(), std::pmr::null_memory_resource()};
    // upstream 用 null：缓冲耗尽会 bad_alloc，便于证明“未走堆”

    {
        std::pmr::vector<int> v{&pool};
        for (int i = 0; i < 32; ++i) {
            v.push_back(i);
        }
        assert(v.size() == 32);
        assert(v[31] == 31);
        std::cout << "pmr::vector size=" << v.size() << " (from stack buffer)\n";
    }

    std::cout << "=== uses-allocator 传播：vector<string> 同 resource ===\n";
    {
        // 注意：pool 不单独释放，继续在同一 arena 上分配
        std::pmr::vector<std::pmr::string> strs{&pool};
        strs.emplace_back("hello");
        strs.emplace_back("pmr");
        assert(strs[0] == "hello");
        std::cout << "nested pmr::string: " << strs[0] << ' ' << strs[1] << '\n';
    }

    std::cout << "=== 缓冲耗尽 → bad_alloc（null upstream） ===\n";
    {
        [[maybe_unused]] bool threw = false;
        try {
            std::pmr::vector<std::byte> huge{&pool};
            // 试图超过剩余缓冲
            huge.resize(4096);
        } catch (const std::bad_alloc&) {
            threw = true;
        }
        assert(threw);
        std::cout << "exhausted buffer threw bad_alloc\n";
    }

    std::cout << "rules:\n";
    std::cout << "  - buffer lifetime must outlive containers using it\n";
    std::cout << "  - monotonic: no per-object free, release all on destroy\n";
    std::cout << "  - ideal for request arenas / frame allocators\n";
    std::cout << "[monotonic_buffer_resource_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/monotonic_buffer_resource_cpp17", run>;

}  // namespace

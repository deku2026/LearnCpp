// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C8 monotonic_buffer_resource)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : monotonic_buffer_resource
// Topic id : part6/c/section06/monotonic_buffer_resource
//
// 要点: 只增不减；可垫栈 buffer；帧/请求级临时分配神器。
// 参考: https://en.cppreference.com/w/cpp/memory/monotonic_buffer_resource

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <memory_resource>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C8 monotonic_buffer_resource ===\n";

    std::array<std::byte, 2048> buffer{};
    std::pmr::monotonic_buffer_resource pool{buffer.data(), buffer.size()};

    {
        std::pmr::vector<int> v{&pool};
        for (int i = 0; i < 100; ++i) v.push_back(i);
        assert(v.size() == 100 && v[42] == 42);

        std::pmr::vector<std::pmr::string> words{&pool};
        words.emplace_back("frame");
        words.emplace_back("temp");
        assert(words[0] == "frame");
        // words/v 析构后块不归还上游，直到 resource 销毁/release
    }

    // 上游耗尽后可再向 next_resource 要（默认 new_delete）
    std::pmr::monotonic_buffer_resource heap_backed{1024};
    std::pmr::vector<int> big{&heap_backed};
    big.resize(200, 1);
    assert(big.size() == 200);

    std::cout << "  bump allocation; no per-block free until resource dies\n";
    std::cout << "  perfect for per-frame / per-request scratch\n";
    std::cout << "monotonic_buffer_resource: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/monotonic_buffer_resource", run>;

}  // namespace

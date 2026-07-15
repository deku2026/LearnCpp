// Topic    : vector 增长策略 / capacity / reserve / resize
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 1.1（验收点）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : vector_growth_reserve
// Topic id : part2/stage10/section01/vector_growth_reserve
// Refs     : https://en.cppreference.com/w/cpp/container/vector
//            https://en.cppreference.com/w/cpp/container/vector/reserve
//            https://en.cppreference.com/w/cpp/container/vector/capacity
//            ISO [vector.capacity]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [vector_growth_reserve] 入门：size vs capacity ===\n";
    {
        std::vector<int> v;
        assert(v.size() == 0);
        assert(v.capacity() == 0);
        assert(v.empty());

        v.push_back(1);
        assert(v.size() == 1);
        assert(v.capacity() >= 1);
        std::cout << "after 1 push: size=" << v.size() << " cap=" << v.capacity() << '\n';
    }

    std::cout << "=== 入门：观察几何增长（实现相关，通常 1.5x/2x）===\n";
    {
        std::vector<int> v;
        std::size_t last_cap = 0;
        int reallocs = 0;
        for (int i = 0; i < 32; ++i) {
            v.push_back(i);
            if (v.capacity() != last_cap) {
                ++reallocs;
                std::cout << "size=" << v.size() << " cap=" << v.capacity() << " (realloc #" << reallocs << ")\n";
                last_cap = v.capacity();
            }
        }
        // 若不预留，从空 vector 推 32 次，必然多次扩容
        assert(reallocs >= 2);
        assert(v.size() == 32);
        assert(v.capacity() >= 32);
    }

    std::cout << "=== 进阶：reserve 预留 → 后续 push 不再 reallocate ===\n";
    {
        std::vector<int> v;
        v.reserve(1000);
        assert(v.size() == 0);  // reserve 只改 capacity
        assert(v.capacity() >= 1000);

        [[maybe_unused]] const auto* data_before = v.data();
        for (int i = 0; i < 1000; ++i) {
            v.push_back(i);
        }
        // 容量足够时 data 指针稳定（标准保证：未超 capacity 时不 reallocate）
        assert(v.data() == data_before);
        assert(v.size() == 1000);
        assert(v.capacity() >= 1000);
        std::cout << "reserved 1000 pushes: data stable, cap=" << v.capacity() << '\n';
    }

    std::cout << "=== 进阶：resize 改 size（构造/析构元素）===\n";
    {
        std::vector<int> v;
        v.resize(5);  // 值初始化 5 个 0
        assert(v.size() == 5);
        assert(v[0] == 0 && v[4] == 0);

        v.resize(3);  // 尾部 2 个元素析构
        assert(v.size() == 3);

        v.resize(6, 42);  // 新增元素填 42
        assert(v.size() == 6);
        assert(v[3] == 42 && v[5] == 42);
        std::cout << "resize: shrink/grow with value-init vs fill value\n";
    }

    std::cout << "=== 专家：shrink_to_fit 请求（非强制）与 capacity 语义 ===\n";
    {
        std::vector<int> v(100);
        v.reserve(1000);
        assert(v.capacity() >= 1000);
        v.resize(10);
        v.shrink_to_fit();  // 非绑定请求：实现可保留额外 capacity
        assert(v.size() == 10);
        std::cout << "after shrink_to_fit: size=" << v.size() << " cap=" << v.capacity() << " (may still be > size)\n";
        // 标准：capacity() >= size() 恒成立
        assert(v.capacity() >= v.size());
    }

    std::cout << "=== 专家：reserve 不足时仍会扩容（边界）===\n";
    {
        std::vector<int> v;
        v.reserve(4);
        [[maybe_unused]] const auto* p = v.data();
        for (int i = 0; i < 4; ++i) {
            v.push_back(i);
        }
        assert(v.data() == p);
        v.push_back(99);  // 超过 4 → 必然 reallocate
        assert(v.data() != p);
        assert(v.capacity() > 4);
        std::cout << "push beyond reserve: reallocated, new cap=" << v.capacity() << '\n';
    }

    std::cout << "[vector_growth_reserve] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01/vector_growth_reserve", run>;

}  // namespace

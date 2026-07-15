// Topic    : vector 迭代器/指针/引用失效（扩容与插入删除）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 1.1 / 4.2（验收点核心）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : vector_iterator_invalidation
// Topic id : part2/stage10/section01/vector_iterator_invalidation
// Refs     : https://en.cppreference.com/w/cpp/container/vector
//            https://en.cppreference.com/w/cpp/container/vector#Iterator_invalidation
//            ISO [vector.modifiers] [vector.capacity]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [vector_iterator_invalidation] 入门：无扩容时 data 稳定 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        v.reserve(16);  // 确保后续 push 不 reallocate
        [[maybe_unused]] int* p = v.data();
        [[maybe_unused]] auto it = v.begin();
        [[maybe_unused]] int& r = v[0];

        v.push_back(4);
        v.push_back(5);
        // capacity 充足：指针/迭代器/引用仍有效
        assert(v.data() == p);
        assert(*it == 1);
        assert(r == 1);
        assert(*p == 1);
        std::cout << "no reallocation: iterator/ref/pointer remain valid\n";
    }

    std::cout << "=== 入门：扩容后旧指针/迭代器全部失效（用 data 对比证明）===\n";
    {
        std::vector<int> v;
        v.reserve(2);
        v.push_back(10);
        v.push_back(20);

        int* old_data = v.data();
        const auto old_cap = v.capacity();
        assert(old_cap >= 2);

        // 强制超出 capacity → reallocate
        // 注意：push 之后恒有 size() <= capacity()，不能用 size<=cap 作循环条件
        while (v.capacity() == old_cap) {
            v.push_back(static_cast<int>(v.size()));
        }
        assert(v.data() != old_data);
        // ⚠️ 此刻 old_data 指向已释放内存；*old_data 是 UB，切勿解引用
        // 我们只比较地址，证明存储位置已变
        std::cout << "reallocated: old_data=" << static_cast<void*>(old_data)
                  << " new_data=" << static_cast<void*>(v.data()) << '\n';
        assert(v.front() == 10);
    }

    std::cout << "=== 进阶：中间 insert 使插入点之后的迭代器失效 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        v.reserve(32);  // 排除 reallocation 干扰
        [[maybe_unused]] auto it_begin = v.begin();
        auto it_mid = v.begin() + 2;  // 指向 3
        auto it_end_1 = v.end() - 1;  // 指向 5

        // 在 mid 前插入 → mid 及之后失效；begin 在无 reallocate 时仍有效
        v.insert(it_mid, 99);
        assert(*it_begin == 1);
        assert(v[2] == 99);
        // it_mid / it_end_1 已失效，不可再使用
        (void)it_end_1;
        std::cout << "after insert at mid (no realloc): begin still valid\n";
        // 正确做法：用 insert 返回值
        auto pos = v.begin() + 1;
        pos = v.insert(pos, 77);
        assert(*pos == 77);
    }

    std::cout << "=== 进阶：erase 使删除点及之后失效；返回下一有效迭代器 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6};
        // 经典 bug：for (++it) 再 erase(it) → 失效后 ++
        // 正确：用 erase 返回值
        for (auto it = v.begin(); it != v.end();) {
            if (*it % 2 == 0) {
                it = v.erase(it);  // 指向被删元素的下一个
            } else {
                ++it;
            }
        }
        assert((v == std::vector<int>{1, 3, 5}));
        std::cout << "erase-while-iterate via return value: odd only\n";

        // C++20 更好：erase_if
        std::vector<int> w{1, 2, 3, 4, 5, 6};
        std::erase_if(w, [](int x) { return x % 2 == 0; });
        assert((w == std::vector<int>{1, 3, 5}));
    }

    std::cout << "=== 专家：失效规则速查（vector）===\n";
    // reallocate（capacity 不足的 insert/push/emplace/reserve 增长…）：
    //   → 所有 iterators / pointers / references 失效
    // 无 reallocate 的 insert：插入点及之后的 iterator/reference 失效
    // erase：被删点及之后的 iterator/reference 失效
    // clear：全部失效（end 除外语义实现相关，勿依赖旧 end）
    std::cout << "realloc: ALL invalidate; insert: after pos; erase: at/after\n";

    std::cout << "=== 专家：持有引用跨可能扩容的 API 是雷 ===\n";
    {
        std::vector<std::string> names{"alice", "bob"};
        names.reserve(2);
        std::string& ref = names[0];
        // 若后续 API 可能 push 超 capacity，ref 会悬空
        // 安全模式：拷贝值，或先 reserve 够，或用下标事后再取
        std::string copy = names[0];
        names.push_back("carol");  // 可能 reallocate
        assert(copy == "alice");
        // 不要在 push 后用 ref
        (void)ref;
        std::cout << "prefer index/copy over long-lived reference into vector\n";
    }

    std::cout << "[vector_iterator_invalidation] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01/vector_iterator_invalidation", run>;

}  // namespace

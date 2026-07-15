// Topic    : 移动后的迭代器/指针/引用失效风险（安全演示）
// Doc      : 第2部分-阶段5 · 步骤 11.3
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : iterator_dangling_after_move
// Topic id : part2/stage05/section05/iterator_dangling_after_move
// Refs     : https://en.cppreference.com/w/cpp/utility/move
//            vector::operator= / move 保证；[lib.types.movedfrom]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iterator_dangling_after_move] 入门：vector 移动 ===\n";
    // 将 vector 移走后：
    // · 源 vector 处于 valid-but-unspecified（通常为空）
    // · 指向「源」的迭代器/引用/指针全部失效，不得再使用
    // · 目标 vector 持有元素；可用目标的新迭代器

    std::vector<int> src{10, 20, 30};
    auto it = src.begin();
    int* ptr = src.data();
    assert(*it == 10 && *ptr == 10);

    std::vector<int> dst = std::move(src);
    assert(dst.size() == 3 && dst[0] == 10);

    // 禁止：*it / *ptr  —— 可能 UAF 或读到已释放缓冲
    // 正确：丢弃旧迭代器，只使用 dst
    assert(dst.front() == 10);
    [[maybe_unused]] auto it2 = dst.begin();
    assert(*it2 == 10);
    std::cout << "after move: use dst iterators only; src iterators discarded\n";
    (void)it;
    (void)ptr;

    // 源可安全做无前置条件操作
    src.clear();
    src.push_back(1);
    assert(src.size() == 1);

    std::cout << "=== 进阶：string / unique_ptr 观察指针 ===\n";
    {
        std::string a = "abcdef";
        // char* p = a.data();  // 移动后不得用 p
        std::string b = std::move(a);
        assert(b == "abcdef");
        // 使用 b.data() 新指针
        assert(b.data()[0] == 'a');
        a = "new";
        assert(a == "new");
        std::cout << "string move: reacquire data() from new owner\n";
    }

    {
        auto p = std::make_unique<int>(5);
        int* raw = p.get();
        auto q = std::move(p);
        assert(p.get() == nullptr);
        // raw 仍指向对象（对象现由 q 拥有）—— 仅在 q 存活期间可观察
        assert(q && *q == 5);
        assert(raw == q.get());
        std::cout << "unique_ptr move: raw borrow valid only while new owner lives\n";
        q.reset();
        // raw 现在悬垂 —— 不访问
        raw = nullptr;
        (void)raw;
    }

    std::cout << "=== 进阶：容器元素被移走 ===\n";
    {
        std::vector<std::string> v{"one", "two", "three"};
        std::string stolen = std::move(v[1]);
        assert(stolen == "two");
        // v[1] 是 moved-from string：可赋值，不可假设内容
        v[1] = "TWO";
        assert(v[1] == "TWO");
        // 指向 v[1] 的迭代器仍有效（元素节点还在，只是值被移走）
        [[maybe_unused]] auto it_elem = v.begin() + 1;
        assert(*it_elem == "TWO");
        std::cout << "moving an element value != moving the container\n";
    }

    std::cout << "=== 专家：规则总结 ===\n";
    // · 移动「容器」：旧缓冲观察者全部作废
    // · 移动「元素」：该元素值 valid-but-unspecified；其它元素与容量结构通常仍在
    // · unique_ptr 移动：旧 unique_ptr 空；裸 get() 观察转跟新所有者
    // · 防御：move 之后立刻停止使用旧迭代器/引用；需要时从新所有者重新取

    std::cout << "=== iterator_dangling_after_move: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section05/iterator_dangling_after_move", run>;

}  // namespace

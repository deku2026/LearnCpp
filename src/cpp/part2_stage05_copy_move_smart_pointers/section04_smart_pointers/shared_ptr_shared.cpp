// Topic    : shared_ptr —— 共享所有权与引用计数
// Doc      : 第2部分-阶段5 · 步骤 10.1
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : shared_ptr_shared
// Topic id : part2/stage05/section04/shared_ptr_shared
// Refs     : https://en.cppreference.com/w/cpp/memory/shared_ptr
//            Effective Modern C++ Item 19

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Widget {
    std::string name;
    explicit Widget(std::string n) : name(std::move(n)) { std::cout << "  +Widget " << name << '\n'; }
    ~Widget() { std::cout << "  -Widget " << name << '\n'; }
};

void share_in(std::shared_ptr<Widget> p) {
    std::cout << "  share_in use_count=" << p.use_count() << '\n';
    assert(p.use_count() >= 2);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [shared_ptr_shared] 入门：共享与 use_count ===\n";

    auto a = std::make_shared<Widget>("A");
    assert(a.use_count() == 1);
    {
        auto b = a;  // 拷贝 → 强引用 +1
        assert(a.use_count() == 2);
        assert(b.use_count() == 2);
        assert(a.get() == b.get());
        std::cout << "count with b=" << a.use_count() << '\n';
        share_in(a);
        assert(a.use_count() == 2);
    }  // b 析构 → 计数 1
    assert(a.use_count() == 1);
    std::cout << "count after b gone=" << a.use_count() << '\n';

    std::cout << "=== 进阶：移动 shared_ptr；容器共享 ===\n";
    {
        auto p = std::make_shared<Widget>("P");
        auto q = std::move(p);
        assert(q && q.use_count() == 1);
        assert(!p);  // 移后为空
        assert(p.use_count() == 0);

        std::vector<std::shared_ptr<Widget>> v;
        v.push_back(q);
        v.push_back(q);
        assert(q.use_count() == 3);
        std::cout << "vector aliases: use_count=" << q.use_count() << '\n';
    }

    std::cout << "=== 进阶：别从同一裸指针构造两个 shared_ptr ===\n";
    {
        // 正确：一份控制块
        auto x = std::make_shared<int>(42);
        auto y = x;
        assert(x.use_count() == 2);

        // 错误示范（切勿执行）:
        // int* raw = new int(1);
        // std::shared_ptr<int> a(raw);
        // std::shared_ptr<int> b(raw); // UB：两套控制块 → 双重 delete
        std::cout << "never construct two shared_ptr from the same raw pointer\n";
    }

    std::cout << "=== 专家：开销与线程 ===\n";
    // · 控制块里强/弱引用计数通常原子增减 → 线程安全地改计数
    // · 同一对象的数据竞争仍需自行同步
    // · 默认优先 unique_ptr；确实共享所有权再用 shared_ptr
    // · 自定义删除器、别名构造、enable_shared_from_this 见同 section 其它 topic

    auto last = std::make_shared<Widget>("last");
    assert(last.use_count() == 1);

    std::cout << "=== shared_ptr_shared: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/shared_ptr_shared", run>;

}  // namespace

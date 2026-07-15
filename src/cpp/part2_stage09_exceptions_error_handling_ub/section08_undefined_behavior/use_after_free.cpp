// Topic    : 释放后使用 UB —— RAII / unique_ptr 对照
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : use_after_free
// Topic id : part2/stage09/section08/use_after_free
// Refs     : https://en.cppreference.com/w/cpp/language/ub
//            ASan use-after-free

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [use_after_free] 主干：unique_ptr 拥有权 ===\n";
    {
        auto p = std::make_unique<int>(42);
        assert(*p == 42);
        p.reset();  // 释放
        assert(p == nullptr);
        // 此后不再解引用 p
        std::cout << "reset leaves nullptr; do not dereference\n";
    }

    std::cout << "=== 对抗：容器与迭代器失效心智 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        auto it = v.begin();
        assert(*it == 1);
        v.push_back(4);  // 可能重分配 → 旧 it 失效（再解引用是 UB）
        // ✅ 失效后重新取迭代器
        it = v.begin();
        assert(*it == 1);
        std::cout << "re-seat iterator after potential reallocation\n";
    }

    std::cout << "=== 进阶：shared_ptr 延长寿命；weak_ptr 观察 ===\n";
    {
        std::shared_ptr<int> owner = std::make_shared<int>(99);
        std::weak_ptr<int> observer = owner;
        assert(!observer.expired());
        {
            auto locked = observer.lock();
            assert(locked && *locked == 99);
        }
        owner.reset();
        assert(observer.expired());  // 不再持有 → 不能当裸指针继续用
        std::cout << "weak_ptr.lock() fails after last owner dies\n";
    }

    std::cout << "=== 专节：UB 形态（不触发）===\n";
    // 危险（勿运行）：
    //   int* p = new int(1); delete p; int x = *p;  // use-after-free
    //   delete p; delete p;                         // double-free
    //   捕获引用/指针到局部后函数返回再解引用
    // 护栏：ASan；纪律：RAII/智能指针，裸 new/delete 最小化。
    std::cout << "RAII + smart pointers; ASan for UAF in tests\n";

    std::cout << "[use_after_free] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/use_after_free", run>;

}  // namespace

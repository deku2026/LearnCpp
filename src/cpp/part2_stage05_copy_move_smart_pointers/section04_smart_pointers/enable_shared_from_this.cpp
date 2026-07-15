// Topic    : enable_shared_from_this —— 从 this 安全取 shared_ptr
// Doc      : 第2部分-阶段5 · 步骤 10.4–10.5
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : enable_shared_from_this
// Topic id : part2/stage05/section04/enable_shared_from_this
// Refs     : https://en.cppreference.com/w/cpp/memory/enable_shared_from_this
//            Effective Modern C++ Item 19

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct Widget : std::enable_shared_from_this<Widget> {
    std::string name;
    explicit Widget(std::string n) : name(std::move(n)) { std::cout << "  +Widget " << name << '\n'; }
    ~Widget() { std::cout << "  -Widget " << name << '\n'; }

    std::shared_ptr<Widget> get_self() { return shared_from_this(); }

    // 错误写法（切勿）: return std::shared_ptr<Widget>(this);
    // 会创建第二套控制块 → 双重释放

    void enroll(std::vector<std::shared_ptr<Widget>>& roster) { roster.push_back(shared_from_this()); }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [enable_shared_from_this] 入门：共享控制块 ===\n";

    auto w = std::make_shared<Widget>("w");
    assert(w.use_count() == 1);

    auto self = w->get_self();
    assert(self.get() == w.get());
    assert(w.use_count() == 2);
    std::cout << "use_count after shared_from_this=" << w.use_count() << '\n';

    std::vector<std::shared_ptr<Widget>> roster;
    w->enroll(roster);
    assert(roster.size() == 1);
    assert(w.use_count() == 3);

    std::cout << "=== 进阶：必须先由 shared_ptr 管理 ===\n";
    {
        bool threw = false;
        try {
            Widget stack("stack");           // 栈对象，无 shared_ptr 管理
            (void)stack.shared_from_this();  // C++17：抛 bad_weak_ptr
        } catch (const std::bad_weak_ptr&) {
            threw = true;
            std::cout << "shared_from_this on unmanaged object -> bad_weak_ptr\n";
        }
        assert(threw);
    }

    std::cout << "=== 进阶：禁止从裸指针再造 shared_ptr ===\n";
    {
        auto a = std::make_shared<Widget>("solo");
        // auto bad = std::shared_ptr<Widget>(a.get()); // UB：第二控制块
        auto ok = a->shared_from_this();
        assert(ok.use_count() == a.use_count());
        std::cout << "shared_from_this shares control block; raw-new shared_ptr does not\n";
    }

    std::cout << "=== 专家：继承注意 ===\n";
    // · 公有继承 enable_shared_from_this<T>，T 通常是最派生类型
    // · 构造函数里不要 shared_from_this（此时控制块可能尚未关联）
    // · weak_from_this()（C++17）可取 weak_ptr 而不提升强计数
    auto w2 = std::make_shared<Widget>("w2");
    std::weak_ptr<Widget> ww = w2->weak_from_this();
    assert(!ww.expired());
    assert(w2.use_count() == 1);

    std::cout << "=== enable_shared_from_this: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/enable_shared_from_this", run>;

}  // namespace

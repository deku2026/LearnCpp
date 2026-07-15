// Topic    : shared_ptr 控制块 —— 双指针、强/弱计数、删除器
// Doc      : 第2部分-阶段5 · 步骤 10.1
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : shared_ptr_control_block
// Topic id : part2/stage05/section04/shared_ptr_control_block
// Refs     : https://en.cppreference.com/w/cpp/memory/shared_ptr
//            ISO [util.smartptr.shared]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

namespace {

int g_deleted = 0;

struct Blob {
    int n;
    explicit Blob(int v) : n(v) {}
    ~Blob() {
        ++g_deleted;
        std::cout << "  ~Blob " << n << '\n';
    }
};

void custom_delete(Blob* p) {
    std::cout << "  custom_delete\n";
    delete p;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [shared_ptr_control_block] 入门：结构 ===\n";
    // shared_ptr 持有两个指针概念上：
    //   1) 指向被管理对象（get()）
    //   2) 指向控制块 { 强计数, 弱计数, 删除器, 分配器... }
    // 拷贝 shared_ptr → 强计数 +1；析构 → 强计数 -1
    // 强计数归零 → 调用删除器销毁对象
    // 弱计数也归零 → 释放控制块本身

    g_deleted = 0;
    std::weak_ptr<Blob> observer;
    {
        auto a = std::make_shared<Blob>(1);
        observer = a;
        assert(a.use_count() == 1);
        assert(!observer.expired());
        {
            auto b = a;
            assert(a.use_count() == 2);
            std::cout << "strong count=2, weak observes same control block\n";
        }
        assert(a.use_count() == 1);
        assert(g_deleted == 0);
    }  // 强计数 0 → 销毁 Blob；弱计数仍 1 → 控制块还在
    assert(g_deleted == 1);
    assert(observer.expired());
    std::cout << "object destroyed; weak expired; control block freed when weak gone\n";

    std::cout << "=== 进阶：自定义删除器存在控制块（类型擦除） ===\n";
    {
        g_deleted = 0;
        std::shared_ptr<Blob> p(new Blob(2), &custom_delete);
        // 删除器类型不进入 shared_ptr<Blob> 的类型
        std::shared_ptr<Blob> q = p;
        assert(p.use_count() == 2);
        q.reset();
        assert(p.use_count() == 1);
    }
    assert(g_deleted == 1);

    std::cout << "=== 进阶：别名构造（aliasing constructor） ===\n";
    {
        auto owner = std::make_shared<std::pair<int, std::string>>(7, "tail");
        // 共享 owner 的控制块，但 get() 指向成员
        std::shared_ptr<std::string> alias(owner, &owner->second);
        assert(alias.use_count() == 2);
        assert(*alias == "tail");
        owner.reset();
        assert(alias.use_count() == 1);
        assert(*alias == "tail");  // pair 仍因 alias 的强计数存活
        std::cout << "alias keeps whole pair alive: " << *alias << '\n';
    }

    std::cout << "=== 专家：make_shared 与控制块布局 ===\n";
    // · make_shared：对象 + 控制块一次分配（见 make_shared_single_allocation）
    // · shared_ptr(new T)：两次分配
    // · 原子引用计数：改计数线程安全；数据竞争另说
    // · sizeof(shared_ptr) 通常是两个指针

    std::cout << "sizeof(shared_ptr<int>)=" << sizeof(std::shared_ptr<int>) << '\n';
    std::cout << "=== shared_ptr_control_block: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/shared_ptr_control_block", run>;

}  // namespace

// Topic    : 临时对象生命周期 —— 默认到完整表达式末尾
// Doc      : 第2部分-阶段5 · 步骤 11.1
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : temporary_object_lifetime
// Topic id : part2/stage05/section05/temporary_object_lifetime
// Refs     : https://en.cppreference.com/w/cpp/language/lifetime
//            ISO [class.temporary]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

int g_alive = 0;

struct Temp {
    int id;
    explicit Temp(int i) : id(i) {
        ++g_alive;
        std::cout << "  +Temp " << id << " alive=" << g_alive << '\n';
    }
    ~Temp() {
        --g_alive;
        std::cout << "  -Temp " << id << " alive=" << g_alive << '\n';
    }
    int value() const { return id; }
};

Temp make_temp(int i) {
    return Temp{i};
}

void take_const_ref(const Temp& t) {
    std::cout << "  take_const_ref " << t.id << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [temporary_object_lifetime] 入门：完整表达式末尾销毁 ===\n";

    g_alive = 0;
    take_const_ref(Temp{1});  // 临时在整条调用语句结束后析构
    assert(g_alive == 0);

    std::cout << "=== 进阶：子表达式中的临时量 ===\n";
    {
        int sum = Temp{2}.value() + Temp{3}.value();
        assert(sum == 5);
        assert(g_alive == 0);  // 语句结束后都销毁
        std::cout << "sum of two temps=" << sum << '\n';
    }

    std::cout << "=== 进阶：初始化与函数返回 prvalue ===\n";
    {
        Temp t = make_temp(4);  // C++17 物化：t 即对象，非「临时再移动」的旧模型
        assert(t.id == 4);
        assert(g_alive == 1);
    }
    assert(g_alive == 0);

    std::cout << "=== 专家：与引用绑定的关系（详见 const_ref_extends_temporary） ===\n";
    // · 默认：临时对象在包含它的完整表达式末尾销毁
    // · 例外：绑定到 const T& / T&& 时可延长到引用生命周期
    // · 延长不「穿透」到成员/下标的二次绑定
    // · 范围 for 的 range 表达式临时量会被延长到循环结束

    {
        std::vector<std::string> v{"a", "b"};
        for (char ch : std::string{"xy"}) {  // 临时 string 活过整个 for
            assert(ch == 'x' || ch == 'y');
            (void)ch;
        }
        (void)v;
        std::cout << "range-for extends temporary range object\n";
    }

    std::cout << "=== temporary_object_lifetime: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section05/temporary_object_lifetime", run>;

}  // namespace

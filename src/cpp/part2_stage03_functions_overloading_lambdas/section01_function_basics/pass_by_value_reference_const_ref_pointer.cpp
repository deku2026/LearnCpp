// Topic     : 传参取舍：值 / 引用 / const 引用 / 指针
// Doc       : 第2部分-阶段3 · 步骤 1
// cppreference: https://en.cppreference.com/cpp/language/reference
//               https://en.cppreference.com/cpp/language/pointer
//               https://en.cppreference.com/cpp/language/value_category
//
// 要点: 只读大对象 → const T&；要改调用者 → T&；小廉价类型 → 传值；
//       可能不存在 / 改指向 → T*；sink 参数按值 + move（预习阶段 5）。
//       ⚠️ 标准 C++ 无 COW，按值传大对象是真拷贝（对比 Qt QString 习惯）。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

void by_value(std::string s) {
    s += " (copy)";
    (void)s;
}

void by_ref(std::string& s) {
    s += " (mutated)";
}

void by_const_ref(const std::string& s) {
    assert(!s.empty() || s.empty());  // 只读；左右值都可绑定
    (void)s;
}

void by_ptr(std::string* s) {
    if (s != nullptr) {
        *s += " (via ptr)";
    }
}

// sink：按值接收，再 move 进成员——左值拷贝一次，右值移动一次
class Widget {
    std::string name_;

public:
    void set_name(std::string name) { name_ = std::move(name); }
    const std::string& name() const { return name_; }
};

[[maybe_unused]] int cheap_by_value(int x) {
    return x + 1;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pass_by_value_reference_const_ref_pointer] ===\n";

    // -------------------------------------------------------------------------
    // §入门：四种形参形态
    // -------------------------------------------------------------------------
    std::string msg = "hello";
    by_value(msg);
    assert(msg == "hello");  // 函数内改的是副本

    by_ref(msg);
    assert(msg == "hello (mutated)");

    by_const_ref(msg);
    by_const_ref(std::string{"temp"});  // 临时右值可绑 const&

    by_ptr(&msg);
    assert(msg == "hello (mutated) (via ptr)");
    by_ptr(nullptr);  // 指针可表达「对象不存在」

    std::cout << "[intro] value/ref/const-ref/pointer semantics OK; msg=\"" << msg << "\"\n";

    // -------------------------------------------------------------------------
    // §进阶：决策表落地 + 小类型传值
    // -------------------------------------------------------------------------
    // 只读大对象：const& 零拷贝
    const std::vector<int> big(1000, 7);
    [[maybe_unused]] auto sum_const_ref = [](const std::vector<int>& v) {
        long long s = 0;
        for (int x : v) {
            s += x;
        }
        return s;
    };
    assert(sum_const_ref(big) == 7000);

    // 小且廉价：传值比间接访问更简单，且无别名
    assert(cheap_by_value(41) == 42);

    // 指针：可选输出 / 可选对象
    std::string* maybe = nullptr;
    by_ptr(maybe);  // no-op
    std::string local = "x";
    maybe = &local;
    by_ptr(maybe);
    assert(local == "x (via ptr)");

    std::cout << "[advanced] const& for large read-only; by-value for cheap types\n";

    // -------------------------------------------------------------------------
    // §专家：sink 参数；引用 vs 指针；Qt 对照边界
    // -------------------------------------------------------------------------
    Widget w;
    std::string owned = "alpha";
    w.set_name(owned);  // 左值 → 拷贝进参数，再 move 进成员
    assert(w.name() == "alpha");
    assert(owned == "alpha");  // 原变量仍在

    w.set_name(std::string{"beta"});  // 右值 → 移动进参数，再 move 进成员
    assert(w.name() == "beta");

    // 引用不可空；指针可空——需要「可能不存在」时用指针 / optional，不要用「空引用」幻想
    // ⚠️ 勿返回指向局部的引用/指针（悬垂，UB）——阶段 5 再系统讲所有权。

    // 🔶 Qt：QString/QList 隐式共享，习惯按值；标准 string/vector 无 COW，
    //    只读大对象默认 const T&，除非 sink 或小对象。
    std::cout << "[expert] sink set_name by-value+move; Qt COW vs std no-COW\n";
    std::cout << "=== pass_by_value_reference_const_ref_pointer: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage03/section01/pass_by_value_reference_const_ref_pointer", run>;

}  // namespace

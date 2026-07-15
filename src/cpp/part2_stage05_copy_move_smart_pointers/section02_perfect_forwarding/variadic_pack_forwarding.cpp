// Topic    : 可变参数包 + 完美转发（make_unique / emplace 核心）
// Doc      : 第2部分-阶段5 · 步骤 6.3–6.4
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : variadic_pack_forwarding
// Topic id : part2/stage05/section02/variadic_pack_forwarding
// Refs     : https://en.cppreference.com/w/cpp/utility/forward
//            https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Widget {
    int id;
    std::string name;

    Widget(int i, std::string n) : id(i), name(std::move(n)) {
        std::cout << "  Widget(" << id << ", " << name << ")\n";
    }
    Widget(const Widget& o) : id(o.id), name(o.name) { std::cout << "  Widget copy\n"; }
    Widget(Widget&& o) noexcept : id(o.id), name(std::move(o.name)) { std::cout << "  Widget move\n"; }
};

// make_unique 本质：参数包完美转发到 new T(...)
template <class T, class... Args>
std::unique_ptr<T> my_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// emplace 风格：在 vector 末尾原位构造
template <class T, class... Args>
T& my_emplace_back(std::vector<T>& v, Args&&... args) {
    v.emplace_back(std::forward<Args>(args)...);
    return v.back();
}

// 统计包大小 / 逐个转发到可调用对象
template <class F, class... Args>
void for_each_arg(F&& f, Args&&... args) {
    // C++17 折叠：把每个 args 完美转发给 f
    (static_cast<void>(std::forward<F>(f)(std::forward<Args>(args))), ...);
}

int g_calls = 0;

void note(const std::string& s) {
    ++g_calls;
    std::cout << "  note lvalue " << s << '\n';
}
void note(std::string&& s) {
    ++g_calls;
    std::cout << "  note rvalue " << s << '\n';
}

// 陷阱演示：花括号列表无法推导
// template<class T> void wrap(T&&); wrap({1,2,3}); // 错误

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [variadic_pack_forwarding] 入门：my_make_unique ===\n";

    std::string name = "alpha";
    auto w1 = my_make_unique<Widget>(1, name);  // name 左值 → 拷贝进 Widget
    assert(w1->id == 1 && w1->name == "alpha");
    assert(name == "alpha");

    auto w2 = my_make_unique<Widget>(2, std::string{"beta"});  // 右值 → 移动
    assert(w2->name == "beta");

    // 对照标准库
    auto w3 = std::make_unique<Widget>(3, "gamma");
    assert(w3->name == "gamma");

    std::cout << "=== 进阶：emplace_back 原位构造 ===\n";
    {
        std::vector<Widget> v;
        v.reserve(4);
        my_emplace_back(v, 10, std::string{"x"});
        std::string y = "y";
        my_emplace_back(v, 11, y);
        assert(v.size() == 2);
        assert(v[0].name == "x" && v[1].name == "y");
        std::cout << "vector size=" << v.size() << '\n';
    }

    std::cout << "=== 进阶：折叠表达式逐个转发 ===\n";
    {
        g_calls = 0;
        std::string a = "A";
        for_each_arg([](auto&& x) { note(std::forward<decltype(x)>(x)); }, a, std::string{"B"}, std::string{"C"});
        assert(g_calls == 3);
    }

    std::cout << "=== 专家：完美转发失败场景（条款 30） ===\n";
    // · 花括号初始化列表无类型 → 无法推导 T
    // · 0/NULL 当空指针 → 推成整型（用 nullptr）
    // · 重载函数名/函数模板名 → 无法确定唯一类型
    // · 仅声明的 static const 整型成员 → 可能需定义
    // 解法：先具名、std::initializer_list、static_cast 选重载等

    auto p = my_make_unique<int>(42);
    assert(p && *p == 42);

    std::cout << "=== variadic_pack_forwarding: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/variadic_pack_forwarding", run>;

}  // namespace

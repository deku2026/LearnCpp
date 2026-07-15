// Topic    : make_unique —— 优先于 unique_ptr(new T)
// Doc      : 第2部分-阶段5 · 步骤 9.1
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : make_unique
// Topic id : part2/stage05/section04/make_unique
// Refs     : https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
//            Effective Modern C++ Item 21

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Widget {
    int id;
    std::string name;
    Widget(int i, std::string n) : id(i), name(std::move(n)) { std::cout << "  Widget(" << id << "," << name << ")\n"; }
};

// 异常安全动机的简化模型：
// process(std::unique_ptr<T>(new T), might_throw());
// 若求值顺序先 new T，再 might_throw() 抛异常，且 unique_ptr 尚未接管 → 泄漏
// make_unique 把分配与托管绑在一次函数调用里，避免该窗口

int might_throw(bool do_throw) {
    if (do_throw) {
        throw std::runtime_error("boom");
    }
    return 1;
}

void process(std::unique_ptr<Widget> w, int) {
    assert(w);
    std::cout << "  process " << w->name << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [make_unique] 入门 ===\n";

    auto p = std::make_unique<int>(42);
    assert(p && *p == 42);

    auto w = std::make_unique<Widget>(1, "alpha");
    assert(w->id == 1 && w->name == "alpha");

    // 数组形式（C++14 make_unique 对数组：make_unique<T[]>(n)）
    auto arr = std::make_unique<int[]>(4);
    arr[0] = 10;
    arr[3] = 40;
    assert(arr[0] == 10 && arr[3] == 40);
    std::cout << "make_unique array arr[3]=" << arr[3] << '\n';

    std::cout << "=== 进阶：异常安全用法 ===\n";
    try {
        process(std::make_unique<Widget>(2, "safe"), might_throw(false));
    } catch (...) {
        assert(false && "should not throw");
    }

    try {
        // 即使 might_throw 抛错，make_unique 的结果作为完整子表达式更易保证不泄漏
        process(std::make_unique<Widget>(3, "temp"), might_throw(true));
        assert(false && "expected throw");
    } catch (const std::runtime_error& e) {
        std::cout << "caught: " << e.what() << " (no leak from make_unique path)\n";
    }

    std::cout << "=== 进阶：自定义参数转发 ===\n";
    {
        std::string name = "beta";
        auto w2 = std::make_unique<Widget>(4, name);
        assert(name == "beta");
        auto w3 = std::make_unique<Widget>(5, std::string{"gamma"});
        assert(w3->name == "gamma");
    }

    std::cout << "=== 专家：何时不能用 make_unique ===\n";
    // · 需要自定义删除器 → unique_ptr<T, D>(ptr, d)
    // · 需要采纳已有裸指针（C API）→ 构造 unique_ptr 并指定删除器
    // · 括号初始化列表直接转发的限制同完美转发
    // · C++14 起标准提供；本仓库 C++23

    std::vector<std::unique_ptr<Widget>> many;
    many.push_back(std::make_unique<Widget>(9, "v"));
    assert(many.size() == 1);

    std::cout << "=== make_unique: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/make_unique", run>;

}  // namespace

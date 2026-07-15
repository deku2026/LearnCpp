// Topic    : 悬垂引用/指针 —— 安全寿命对照
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : dangling_reference
// Topic id : part2/stage09/section08/dangling_reference
// Refs     : https://en.cppreference.com/w/cpp/language/ub
//            https://en.cppreference.com/w/cpp/language/reference
//            ASan

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

// ✅ 返回值（拷贝/移动），不返回局部引用
std::string make_name() {
    std::string local = "Ada";
    return local;
}

// ✅ 返回指向存活对象的引用（调用方保证参数寿命）
const std::string& pick_longer(const std::string& a, const std::string& b) {
    return a.size() >= b.size() ? a : b;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [dangling_reference] 主干：返回值代替返回局部引用 ===\n";
    {
        const std::string name = make_name();
        assert(name == "Ada");
        std::cout << "make_name -> " << name << '\n';
    }

    std::cout << "=== 对抗：引用参数在实参存活期内使用 ===\n";
    {
        const std::string a = "hi";
        const std::string b = "hello";
        const std::string& r = pick_longer(a, b);
        assert(r == "hello");
        std::cout << "longer=" << r << '\n';
    }

    std::cout << "=== 对抗：容器元素引用在重分配前使用 ===\n";
    {
        std::vector<std::string> v{"x", "y"};
        std::string& front = v.front();
        assert(front == "x");
        // 若此处 push 导致重分配，front 悬垂。本 demo 只读不扩容。
        front = "z";
        assert(v[0] == "z");
        std::cout << "reference into vector valid while no reallocation\n";
    }

    std::cout << "=== 专节：UB 形态（不触发）===\n";
    // 危险（勿运行）：
    //   int& f() { int x = 1; return x; }  // 返回局部引用
    //   std::string& r = v[0]; v.push_back(...); use(r);
    // 另见 range_for_temporary_dangle_pre_cpp23
    std::cout << "never return ref/pointer to locals; watch container invalidation\n";

    std::cout << "[dangling_reference] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/dangling_reference", run>;

}  // namespace

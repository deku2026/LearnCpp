// Topic    : C++23 auto(x)/auto{x}：语言级 decay-copy
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 6.5
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : auto_decay_copy_cpp23
// Topic id : part2/stage02/section06/auto_decay_copy_cpp23
// Refs     : https://en.cppreference.com/cpp/language/explicit_cast
//            https://en.cppreference.com/cpp/types/decay
//            提案 P0849R8

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

void inc_print(int& x, const int& y) {
    ++x;
    std::cout << "x:" << x << ", y:" << y << '\n';
}

// 删除所有等于首元素的项：必须先复制 front，避免 erase 中引用失效
void pop_all_equal_to_front(std::vector<int>& v) {
    if (v.empty()) {
        return;
    }
    // auto(v.front())：先造独立 prvalue 副本再传给 erase
    std::erase(v, auto(v.front()));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [auto_decay_copy_cpp23] 入门：切断别名的 prvalue 副本 ===\n";
    {
        // 官方动机：y 若绑定到同一对象，++x 会改 y
        int p{1};
        inc_print(p, p);  // x:2, y:2 —— 别名

        int q{1};
        inc_print(q, auto{q});  // x:2, y:1 —— auto{q} 是独立副本
        assert(q == 2);

        // auto(x)/auto{x} ≈ T(x)/T{x}，T = decay_t<decltype(x)>，结果是 prvalue
        const int& cr = q;
        auto copy = auto(cr);
        static_assert(std::is_same_v<decltype(copy), int>);
        static_assert(std::is_same_v<decltype(auto(cr)), int>);
        // decltype(auto(cr)) 是 int（prvalue 类型），不是引用
        assert(copy == 2);
        std::cout << "[intro] auto{q} breaks aliasing; y sees snapshot\n";
    }

    std::cout << "=== 进阶：等价于 decay_t 的显式转换；数组/函数退化 ===\n";
    {
        int arr[3]{1, 2, 3};
        // 按值 auto 会把数组退化为指针；auto(arr) 同样 decay
        auto ptr_copy = auto(arr);
        static_assert(std::is_same_v<decltype(ptr_copy), int*>);
        assert(ptr_copy[0] == 1);

        // 手写等价物（啰嗦）：
        using T = std::decay_t<decltype(arr)>;
        [[maybe_unused]] T manual = static_cast<T>(arr);
        static_assert(std::is_same_v<T, int*>);
        assert(manual == ptr_copy);

        std::string s = "decay";
        std::string moved_in = auto(std::move(s));  // 仍是 string 的 prvalue 初始化
        // 注意：auto(std::move(s)) 先得到 string 的 prvalue（可能移动构造）
        assert(moved_in == "decay");
        std::cout << "[advanced] auto(x) == decay_t copy as prvalue; no need to name T\n";
    }

    std::cout << "=== 专家：erase 前复制 front；feature 探测 ===\n";
    {
        std::vector<int> v{7, 1, 7, 2, 7};
        // 错误写法风险：std::erase(v, v.front()) 中 front() 引用可能在重分配/移动中悬垂
        // 正确：auto(v.front()) 先拷贝关键值 7
        pop_all_equal_to_front(v);
        assert((v == std::vector<int>{1, 2}));

        std::vector<int> one{5, 5, 5};
        pop_all_equal_to_front(one);
        assert(one.empty());

#if defined(__cpp_auto_cast)
        std::cout << "[expert] __cpp_auto_cast=" << __cpp_auto_cast << '\n';
#else
        std::cout << "[expert] __cpp_auto_cast not defined (compiler may still support auto(x))\n";
#endif

        std::cout << "[expert] use auto(v.front()) before mutating container\n";
    }

    std::cout << "[auto_decay_copy_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/auto_decay_copy_cpp23", run>;

}  // namespace

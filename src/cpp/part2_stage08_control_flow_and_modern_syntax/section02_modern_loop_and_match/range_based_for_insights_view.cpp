// Topic    : 范围 for（展开为 begin/end + 迭代器循环）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 2
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : range_based_for_insights_view
// Topic id : part2/stage08/section02/range_based_for_insights_view
// Refs     : https://en.cppreference.com/w/cpp/language/range-for
//            ISO [stmt.ranged]；C++ Insights 验收点：__range / __begin / __end

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

// 自定义可迭代类型：成员 begin/end + 最小迭代器协议（*、++、!=）
struct IntRange {
    int from{};
    int to{};  // 半开区间 [from, to)

    struct Iterator {
        int value{};
        int operator*() const { return value; }
        Iterator& operator++() {
            ++value;
            return *this;
        }
        bool operator!=(const Iterator& o) const { return value != o.value; }
    };

    Iterator begin() const { return Iterator{from}; }
    Iterator end() const { return Iterator{to}; }
};

// 仅靠 ADL 自由函数 begin/end 支持 range-for（无成员 begin/end）
struct AdlOnlySpan {
    const int* data{};
    std::size_t n{};
};

struct AdlIt {
    const int* p{};
    int operator*() const { return *p; }
    AdlIt& operator++() {
        ++p;
        return *this;
    }
    bool operator!=(const AdlIt& o) const { return p != o.p; }
};

AdlIt begin(const AdlOnlySpan& s) {
    return AdlIt{s.data};
}
AdlIt end(const AdlOnlySpan& s) {
    return AdlIt{s.data + s.n};
}

// 等价展开示意（教学用手写，非编译器真实符号名）
[[maybe_unused]] int sum_like_range_for(const std::vector<int>& v) {
    int sum = 0;
    {
        auto&& __range = v;  // ① 绑定范围（转发引用）
        auto __begin = __range.begin();
        auto __end = __range.end();
        for (; __begin != __end; ++__begin) {
            int x = *__begin;  // ② 解引用初始化循环变量
            sum += x;
        }
    }
    return sum;
}

template <typename Range>
int sum_generic(Range&& r) {
    int s = 0;
    // 泛型代码优先 auto&&：适配值/引用/proxy（如 vector<bool>）
    for (auto&& elem : r) {
        s += static_cast<int>(elem);
    }
    return s;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [range_based_for_insights_view] 入门：值 / 引用 / const 引用 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4, 5};

        [[maybe_unused]] int by_value = 0;
        for (int x : v) {
            by_value += x;
        }
        assert(by_value == 15);

        for (int& x : v) {
            x *= 2;  // 修改元素
        }
        assert((v == std::vector<int>{2, 4, 6, 8, 10}));

        int by_cref = 0;
        for (const int& x : v) {
            by_cref += x;
        }
        assert(by_cref == 30);
        std::cout << "value/ref/const-ref walk ok, sum=" << by_cref << '\n';

        // 原生数组天然支持（情形 1：数组 begin/end 指针算术）
        int arr[] = {7, 8, 9};
        [[maybe_unused]] int arr_sum = 0;
        for (int x : arr) {
            arr_sum += x;
        }
        assert(arr_sum == 24);
    }

    std::cout << "=== 进阶：展开调用了什么 + 自定义 begin/end ===\n";
    {
        std::vector<int> v{1, 2, 3};
        assert(sum_like_range_for(v) == 6);
        std::cout << "hand-expanded __range/__begin/__end sum=6\n";
        std::cout << "calls: begin/end, iterator !=, ++, *\n";

        // 成员 begin/end
        int custom = 0;
        for (int x : IntRange{1, 5}) {
            custom += x;  // 1+2+3+4
        }
        assert(custom == 10);
        std::cout << "member begin/end IntRange sum=" << custom << '\n';

        // ADL 自由函数 begin/end
        const int buf[] = {3, 3, 3};
        AdlOnlySpan span{buf, 3};
        int adl_sum = 0;
        for (int x : span) {
            adl_sum += x;
        }
        assert(adl_sum == 9);
        std::cout << "ADL begin/end span sum=" << adl_sum << '\n';

        // 大对象：只用 const auto&，避免按值拷贝
        std::vector<std::string> names{"alpha", "beta"};
        [[maybe_unused]] std::size_t chars = 0;
        for (const auto& name : names) {
            chars += name.size();
        }
        assert(chars == 9);
    }

    std::cout << "=== 专家：三种 begin 解析、auto&&、失效与 Qt 对照要点 ===\n";
    {
        // begin-expr / end-expr 解析顺序（cppreference）：
        // 1) 数组 → 指针算术
        // 2) 类型有成员 begin 且有成员 end → 成员调用
        // 3) 否则 ADL 找 begin(range)/end(range)
        // 因此自定义类型必须提供其中一种，否则编译失败。

        std::vector<int> left{1, 2, 3};
        assert(sum_generic(left) == 6);
        assert(sum_generic(std::vector<int>{4, 5}) == 9);

        // ⚠️ 范围 for 中增删容器元素可能导致迭代器失效 → UB。
        // 本文件不触发 UB：改为“先收集再处理”模式说明。
        std::vector<int> data{1, 2, 3, 4};
        std::vector<int> to_drop;
        for (int x : data) {
            if (x % 2 == 0) {
                to_drop.push_back(x);
            }
        }
        // 真实删除在第二趟
        data.erase(std::remove_if(data.begin(), data.end(), [](int x) { return x % 2 == 0; }), data.end());
        assert((data == std::vector<int>{1, 3}));
        assert(to_drop.size() == 2);
        std::cout << "do not mutate container size inside range-for\n";

        // 把 for (int x : v) 丢进 C++ Insights，可见隐藏 __range/__begin/__end。
        // 🔶 Qt：QList/QVector 支持 begin/end；只读遍历用 const 或 as_const，
        //    避免对非 const Qt 容器 auto& 触发 COW detach。
        std::cout << "see C++ Insights for desugaring; prefer auto&& in generics\n";
    }

    std::cout << "[range_based_for_insights_view] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/range_based_for_insights_view", run>;

}  // namespace

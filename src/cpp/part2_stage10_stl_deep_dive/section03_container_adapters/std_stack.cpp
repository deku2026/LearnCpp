// Topic    : std::stack：LIFO 容器适配器
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 3.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_stack
// Topic id : part2/stage10/section03/std_stack
// Refs     : https://en.cppreference.com/w/cpp/container/stack
//            ISO [stack]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <deque>
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_stack] 入门：LIFO push/pop/top ===\n";
    {
        std::stack<int> st;
        assert(st.empty());
        st.push(1);
        st.push(2);
        st.push(3);
        assert(st.size() == 3);
        assert(st.top() == 3);
        st.pop();  // 不返回值
        assert(st.top() == 2);
        st.top() = 20;  // top 可写
        assert(st.top() == 20);
        std::cout << "stack: last-in first-out; pop discards\n";
    }

    std::cout << "=== 进阶：底层容器默认 deque，可换 vector/list ===\n";
    {
        std::stack<int, std::vector<int>> st_vec;
        st_vec.push(10);
        st_vec.push(20);
        assert(st_vec.top() == 20);

        std::stack<std::string, std::deque<std::string>> st_deq;
        st_deq.emplace("hello");
        st_deq.emplace("world");
        assert(st_deq.top() == "world");
        std::cout << "adapter: stack<T, Container=deque<T>>\n";
    }

    std::cout << "=== 进阶：表达式求值 / 括号匹配小例子 ===\n";
    {
        auto balanced = [](std::string_view s) {
            std::stack<char> st;
            for (char c : s) {
                if (c == '(' || c == '[' || c == '{') {
                    st.push(c);
                } else if (c == ')' || c == ']' || c == '}') {
                    if (st.empty()) {
                        return false;
                    }
                    const char o = st.top();
                    st.pop();
                    if ((c == ')' && o != '(') || (c == ']' && o != '[') || (c == '}' && o != '{')) {
                        return false;
                    }
                }
            }
            return st.empty();
        };
        assert(balanced("([]){}"));
        assert(!balanced("([)]"));
        assert(!balanced("("));
        std::cout << "classic paren matching via stack\n";
    }

    std::cout << "=== 专家：受限接口 — 无迭代器，不能遍历中间 ===\n";
    {
        std::stack<int> st;
        for (int i = 0; i < 5; ++i) {
            st.push(i);
        }
        // 只能从 top 逐个弹
        std::vector<int> popped;
        while (!st.empty()) {
            popped.push_back(st.top());
            st.pop();
        }
        assert((popped == std::vector<int>{4, 3, 2, 1, 0}));
        std::cout << "no begin/end: API intentionally restricted\n";
    }

    std::cout << "=== 专家：比较运算符比较底层序列 ===\n";
    {
        std::stack<int> a, b;
        a.push(1);
        a.push(2);
        b.push(1);
        b.push(2);
        assert(a == b);
        b.push(3);
        assert(a < b);
        std::cout << "stacks are comparable if elements are\n";
    }

    std::cout << "[std_stack] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03/std_stack", run>;

}  // namespace

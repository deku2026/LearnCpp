// Topic    : std::forward_like（C++23）—— 按模型对象的值类别转发成员
// Doc      : 第2部分-阶段5 · 步骤 7
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : forward_like_cpp23
// Topic id : part2/stage05/section02/forward_like_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/forward_like
//            P2445R1；配合 deducing this

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <version>

namespace {

#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L

struct Scheduler {
    bool submit(const std::string& s) {
        std::cout << "  submit(copy) " << s << '\n';
        last = s;
        return true;
    }
    bool submit(std::string&& s) {
        std::cout << "  submit(move) " << s << '\n';
        last = std::move(s);
        return true;
    }
    std::string last;
};

// 按「拥有者」U 的 const/值类别，转发 member
template <class U, class M>
void show_category(M&& m) {
    using R = decltype(std::forward_like<U>(m));
    if constexpr (std::is_lvalue_reference_v<R>) {
        if constexpr (std::is_const_v<std::remove_reference_t<R>>) {
            std::cout << "  forward_like -> const lvalue ref\n";
        } else {
            std::cout << "  forward_like -> lvalue ref\n";
        }
    } else {
        if constexpr (std::is_const_v<std::remove_reference_t<R>>) {
            std::cout << "  forward_like -> const rvalue ref\n";
        } else {
            std::cout << "  forward_like -> rvalue ref\n";
        }
    }
}

struct Holder {
    std::string member = "payload";

    // deducing this：self 携带调用方的值类别
    template <class Self>
    auto extract(this Self&& self) -> decltype(auto) {
        return std::forward_like<Self>(self.member);
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [forward_like_cpp23] 入门：按模型对象转发 ===\n";
    std::cout << "__cpp_lib_forward_like=" << __cpp_lib_forward_like << '\n';

    std::string m = "msg";
    show_category<std::string&>(m);   // 模型是左值
    show_category<std::string&&>(m);  // 模型是右值
    show_category<const std::string&>(m);

    std::cout << "=== 进阶：deducing this 回调（可重试 vs 用一次） ===\n";
    {
        Scheduler scheduler;
        auto callback = [m = std::string("job"), &scheduler](this auto&& self) -> bool {
            return scheduler.submit(std::forward_like<decltype(self)>(m));
        };

        assert(callback());  // 闭包左值 → 拷贝提交
        assert(scheduler.last == "job");
        assert(callback());  // 仍可再试

        assert(std::move(callback)());  // 闭包右值 → 移动提交
        // 移动后 m 为 moved-from；不再依赖其值
        std::cout << "scheduler.last=" << scheduler.last << '\n';
    }

    std::cout << "=== 进阶：Holder::extract ===\n";
    {
        Holder h;
        std::string& lr = h.extract();  // 左值 this → 左值成员
        lr = "changed";
        assert(h.member == "changed");

        Holder h2;
        std::string moved = std::move(h2).extract();  // 右值 this → 移动成员
        assert(moved == "payload");
        std::cout << "extract from rvalue Holder moved member out\n";
    }

    std::cout << "=== 专家：为何不只写 forward<Self>(self).member ===\n";
    // · 成员访问不一定正确传播值类别（operator*、optional 访问器、容器下标等）
    // · forward_like 专门合并模型类型的 cvref 到任意表达式
    // · 是 C++23 deducing this 的配套拼图

    std::cout << "=== forward_like_cpp23: OK ===\n";
    return 0;
}

#else

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [forward_like_cpp23] std::forward_like not available ===\n";
    std::cout << "need C++23 library feature __cpp_lib_forward_like >= 202207\n";
    return 0;
}

#endif

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/forward_like_cpp23", run>;

}  // namespace

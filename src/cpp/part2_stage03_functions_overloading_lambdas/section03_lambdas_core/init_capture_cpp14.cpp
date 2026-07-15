// Topic     : 初始化捕获 init-capture（C++14）
// Doc       : 第2部分-阶段3 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: [x = expr] 在闭包里新建成员；可 std::move 不可拷贝对象；可计算新值；
//       是 move-only lambda 的关键拼图。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [init_capture_cpp14] ===\n";

    // -------------------------------------------------------------------------
    // §入门：计算新成员
    // -------------------------------------------------------------------------
    int a = 10;
    auto twice = [x = a * 2] { return x; };
    assert(twice() == 20);
    a = 0;
    assert(twice() == 20);
    std::cout << "[intro] [x = expr] creates a new data member initialized from expr\n";

    // -------------------------------------------------------------------------
    // §进阶：move 进闭包
    // -------------------------------------------------------------------------
    auto p = std::make_unique<int>(42);
    auto owns = [ptr = std::move(p)] { return *ptr; };
    assert(p == nullptr);
    assert(owns() == 42);

    std::string s = "payload";
    auto took = [buf = std::move(s)] { return buf.size(); };
    assert(s.empty());  // 被移走（合法 moved-from 状态）
    assert(took() == 7);
    std::cout << "[advanced] move unique_ptr/string into closure via init-capture\n";

    // -------------------------------------------------------------------------
    // §专家：命名、泛型 init-capture（C++14）、与 [=] 区别
    // -------------------------------------------------------------------------
    // 成员名可以与外部不同：[val = std::move(s)]
    // C++14 起也可用 auto 推导：但左侧名字是成员标识符，右侧是表达式。
    // 广义捕获不能写 [p] 当 p 不可拷贝——必须 [p = std::move(p)]。
    // 打包捕获见 capture_pack_cpp20。
    int n = 1;
    auto counter = [k = n]() mutable { return ++k; };
    assert(counter() == 2);
    assert(counter() == 3);
    assert(n == 1);

    std::cout << "[expert] init-capture enables move-only state inside lambdas\n";
    std::cout << "=== init_capture_cpp14: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/init_capture_cpp14", run>;

}  // namespace

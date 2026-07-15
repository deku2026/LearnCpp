// Topic     : 初始化捕获 init-capture（C++14）
// Doc       : 第2部分-阶段3 · 步骤 5.3 / 验收「初始化捕获 move unique_ptr」
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: [x = expr] 在闭包内新建成员，可 std::move 进不可拷贝对象；可计算新值；
//       是 move-only lambda 的关键拼图（再装进 move_only_function）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

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
    assert(s.empty());  // 合法 moved-from 状态
    assert(took() == 7);

    // 只抓字段，避免拷贝整个大对象
    struct Blob {
        int id = 1;
        std::string name = "blob";
        std::vector<int> data{1, 2, 3, 4, 5};
    };
    Blob b;
    auto thin = [id = b.id, name = b.name] { return name + "#" + std::to_string(id); };
    assert(thin() == "blob#1");
    std::cout << "[advanced] move unique_ptr/string into closure; thin field capture\n";

    // -------------------------------------------------------------------------
    // §专家：成员名可与外部不同；mutable；不能 [p] 拷贝 unique_ptr
    // -------------------------------------------------------------------------
    // 成员名可不同于外部：[val = std::move(s)]
    // 不能指望写 [p] 当 p 不可拷贝——必须 [p = std::move(p)]。
    int n = 1;
    auto counter = [k = n]() mutable { return ++k; };
    assert(counter() == 2);
    assert(counter() == 3);
    assert(n == 1);

    // 广义捕获 + 再 move 进另一个包装（预习 move_only_function）
    auto q = std::make_unique<std::string>("task");
    auto work = [msg = std::move(q)]() mutable {
        assert(msg);
        auto out = *msg;
        msg.reset();
        return out;
    };
    assert(work() == "task");

    // C++20 包捕获见 capture_pack_cpp20；C++17 [*this] 见 capture_star_this_cpp17
    std::cout << "[expert] init-capture enables move-only state inside lambdas\n";
    std::cout << "=== init_capture_cpp14: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/init_capture_cpp14", run>;

}  // namespace

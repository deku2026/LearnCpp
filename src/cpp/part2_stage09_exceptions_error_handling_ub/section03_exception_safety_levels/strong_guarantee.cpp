// Topic    : Strong 强异常保证：要么成功，要么完全回滚
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 3.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : strong_guarantee
// Topic id : part2/stage09/section03/strong_guarantee
// Refs     : https://en.cppreference.com/w/cpp/language/exceptions
//            https://en.cppreference.com/w/cpp/container/vector/push_back

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

// 演示：在副本上完成可能失败的工作，成功后再提交
class Document {
    std::string title_;
    std::vector<int> body_;

public:
    Document() = default;
    explicit Document(std::string t) : title_(std::move(t)) {}

    const std::string& title() const noexcept { return title_; }
    const std::vector<int>& body() const noexcept { return body_; }

    // 强保证：append 失败时 *this 不变
    void append_checked(int v, bool fail) {
        auto draft = body_;  // 可能抛（分配），但失败时 *this 未改
        draft.push_back(v);  // 可能抛
        if (fail) {
            throw std::runtime_error("append rejected");
        }
        body_.swap(draft);  // 假设 swap 不抛 → 提交
    }

    void rename(std::string t) {
        // string 赋值在实现上通常提供强保证（或至少 basic）；
        // 这里用临时 + swap 明确强保证。
        std::string draft = std::move(t);
        title_.swap(draft);
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [strong_guarantee] 主干：失败则原对象不变 ===\n";
    {
        Document doc{"v1"};
        doc.append_checked(10, /*fail=*/false);
        doc.append_checked(20, /*fail=*/false);
        assert(doc.body().size() == 2);
        assert(doc.body()[0] == 10 && doc.body()[1] == 20);

        try {
            doc.append_checked(30, /*fail=*/true);
            assert(false);
        } catch (const std::runtime_error& e) {
            std::cout << "failed append: " << e.what() << '\n';
        }
        // 强保证：失败后 body 仍是 {10,20}
        assert(doc.body().size() == 2);
        assert(doc.body()[0] == 10 && doc.body()[1] == 20);
        std::cout << "after failed append, body size still " << doc.body().size() << '\n';
    }

    std::cout << "=== 对抗：vector::push_back 的强保证心智 ===\n";
    {
        std::vector<int> v{1, 2, 3};
        const auto cap = v.capacity();
        v.push_back(4);
        assert(v.size() == 4);
        // 标准保证：若 push_back 因分配失败抛 bad_alloc，原 vector 保持不变
        (void)cap;
        std::cout << "vector push_back documents strong guarantee on alloc fail\n";
    }

    std::cout << "=== 专节：实现套路 ===\n";
    // 1) 在旁路/副本上做可能抛的工作
    // 2) 用 noexcept 的 swap/move 原子提交
    // 3) 详见 copy_and_swap_strong_guarantee
    Document d{"old"};
    d.rename("new");
    assert(d.title() == "new");
    std::cout << "rename via temporary+swap: " << d.title() << '\n';

    std::cout << "[strong_guarantee] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/strong_guarantee", run>;

}  // namespace

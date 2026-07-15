// Topic     : 拷贝赋值运算符
// Doc       : 第2部分-阶段4 · 步骤 3.1
// cppreference: https://en.cppreference.com/cpp/language/copy_assignment
//
// 要点: operator=(const T&) 给已存在对象赋新值；处理自赋值；先释放旧资源再取新资源
//       或 copy-and-swap；返回 *this 支持链式赋值。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace {

class DeepBuffer {
    std::size_t n_ = 0;
    char* data_ = nullptr;

public:
    DeepBuffer() = default;

    explicit DeepBuffer(std::string_view s) : n_(s.size()), data_(new char[n_ + 1]) {
        std::memcpy(data_, s.data(), n_);
        data_[n_] = '\0';
    }

    DeepBuffer(const DeepBuffer& other) : n_(other.n_), data_(other.data_ ? new char[n_ + 1] : nullptr) {
        if (data_) {
            std::memcpy(data_, other.data_, n_ + 1);
        }
    }

    DeepBuffer& operator=(const DeepBuffer& other) {
        if (this == &other) {
            return *this;  // 自赋值
        }
        // 强异常安全简化版：先分配再释放
        char* neu = nullptr;
        if (other.data_) {
            neu = new char[other.n_ + 1];
            std::memcpy(neu, other.data_, other.n_ + 1);
        }
        delete[] data_;
        data_ = neu;
        n_ = other.n_;
        return *this;
    }

    ~DeepBuffer() { delete[] data_; }

    const char* c_str() const { return data_ ? data_ : ""; }
    std::size_t size() const { return n_; }
};

struct Point {
    int x = 0;
    int y = 0;
    // 隐式拷贝赋值：逐成员
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [copy_assignment] ===\n";

    // -------------------------------------------------------------------------
    // §入门：链式赋值 + 隐式
    // -------------------------------------------------------------------------
    Point a{1, 2}, b, c;
    c = b = a;
    assert(b.x == 1 && c.y == 2);
    std::cout << "[intro] chain assign c=(" << c.x << "," << c.y << ")\n";

    // -------------------------------------------------------------------------
    // §进阶：深拷贝赋值 + 自赋值
    // -------------------------------------------------------------------------
    DeepBuffer s{"alpha"};
    DeepBuffer t{"beta"};
    t = s;
    assert(std::string(t.c_str()) == "alpha");
    assert(s.c_str() != t.c_str());

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"
#endif
    t = t;  // 自赋值安全
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    assert(std::string(t.c_str()) == "alpha");
    std::cout << "[advanced] deep assign + self-assign ok\n";

    // -------------------------------------------------------------------------
    // §专家：copy-and-swap 形态（强异常安全惯用法示意）
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto copy_and_swap_assign = [](DeepBuffer& lhs, DeepBuffer rhs) {  // rhs 按值=拷贝
        // 真实类里常写：DeepBuffer& operator=(DeepBuffer other) { swap(*this,other); return *this; }
        DeepBuffer tmp{rhs};
        lhs = tmp;
        return lhs.size();
    };
    DeepBuffer u{"gamma"};
    assert(copy_and_swap_assign(u, DeepBuffer{"delta"}) == 5);
    assert(std::string(u.c_str()) == "delta");

    // 声明拷贝赋值会抑制隐式移动
    std::cout << "[expert] copy-and-swap style size=" << u.size() << '\n';
    std::cout << "=== copy_assignment: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/copy_assignment", run>;

}  // namespace

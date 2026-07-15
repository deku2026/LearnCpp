// Topic     : 移动赋值运算符
// Doc       : 第2部分-阶段4 · 步骤 3.1
// cppreference: https://en.cppreference.com/cpp/language/move_assignment
//
// 要点: operator=(T&&) 释放己方资源后接管源资源；自移动赋值应安全；宜 noexcept。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

class Buffer {
    std::size_t n_ = 0;
    int* data_ = nullptr;

public:
    Buffer() = default;
    explicit Buffer(std::size_t n, int fill = 0) : n_(n), data_(new int[n]) {
        for (std::size_t i = 0; i < n_; ++i) {
            data_[i] = fill;
        }
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& o) noexcept : n_(std::exchange(o.n_, 0)), data_(std::exchange(o.data_, nullptr)) {}

    Buffer& operator=(Buffer&& o) noexcept {
        if (this != &o) {
            delete[] data_;
            n_ = std::exchange(o.n_, 0);
            data_ = std::exchange(o.data_, nullptr);
        }
        return *this;
    }

    ~Buffer() { delete[] data_; }

    std::size_t size() const { return n_; }
    int at(std::size_t i) const { return data_[i]; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [move_assignment] ===\n";

    // -------------------------------------------------------------------------
    // §入门：移动赋值转移所有权
    // -------------------------------------------------------------------------
    Buffer a{4, 2};
    Buffer b{1, 9};
    b = std::move(a);
    assert(b.size() == 4);
    assert(b.at(0) == 2);
    assert(a.size() == 0);
    std::cout << "[intro] move-assign b.size=" << b.size() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：链式与覆盖旧资源
    // -------------------------------------------------------------------------
    Buffer c{2, 3};
    Buffer d{3, 4};
    Buffer e;
    e = std::move(c);
    e = std::move(d);  // 释放原 e 资源，接管 d
    assert(e.size() == 3 && e.at(0) == 4);
    assert(c.size() == 0 && d.size() == 0);
    std::cout << "[advanced] re-assign frees previous resource\n";

    // -------------------------------------------------------------------------
    // §专家：自移动赋值
    // -------------------------------------------------------------------------
    Buffer f{5, 1};
    f = std::move(f);  // 标准要求程序应能处理；我们的实现 this==&o 直接返回
    // 自移动后状态：我们保持原样
    assert(f.size() == 5);
    assert(f.at(0) == 1);

    static_assert(std::is_nothrow_move_assignable_v<Buffer>);
    std::cout << "[expert] self-move-assign safe size=" << f.size() << '\n';
    std::cout << "=== move_assignment: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/move_assignment", run>;

}  // namespace

// Topic     : 移动构造函数
// Doc       : 第2部分-阶段4 · 步骤 3.1（完整移动语义见阶段 5）
// cppreference: https://en.cppreference.com/cpp/language/move_constructor
//
// 要点: T(T&&) 从右值「窃取」资源；源置于有效可析构状态；通常 noexcept；
//       用户声明拷贝/析构会抑制隐式移动。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

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

    Buffer(const Buffer& o) : n_(o.n_), data_(o.data_ ? new int[n_] : nullptr) {
        for (std::size_t i = 0; i < n_; ++i) {
            data_[i] = o.data_[i];
        }
    }

    Buffer(Buffer&& o) noexcept : n_(std::exchange(o.n_, 0)), data_(std::exchange(o.data_, nullptr)) {}

    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    ~Buffer() { delete[] data_; }

    std::size_t size() const { return n_; }
    const int* data() const { return data_; }
    int* data() { return data_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [move_constructor] ===\n";

    // -------------------------------------------------------------------------
    // §入门：移动后源为空、目标持有资源
    // -------------------------------------------------------------------------
    Buffer a{3, 7};
    int* raw = a.data();
    Buffer b{std::move(a)};
    assert(b.size() == 3);
    assert(b.data() == raw);
    assert(a.size() == 0);
    assert(a.data() == nullptr);
    assert(b.data()[0] == 7);
    std::cout << "[intro] moved size=" << b.size() << " src empty=" << (a.data() == nullptr) << '\n';

    // -------------------------------------------------------------------------
    // §进阶：临时对象 / 容器增长触发移动
    // -------------------------------------------------------------------------
    std::vector<Buffer> v;
    v.reserve(2);
    v.push_back(Buffer{2, 1});  // 移动进 vector
    assert(v[0].size() == 2);
    v.emplace_back(4, 5);
    assert(v[1].data()[0] == 5);
    std::cout << "[advanced] vector holds moved/emplaced buffers n=" << v.size() << '\n';

    // -------------------------------------------------------------------------
    // §专家：noexcept 与「有效但未指定」状态
    // -------------------------------------------------------------------------
    static_assert(std::is_nothrow_move_constructible_v<Buffer>);
    Buffer c{std::move(b)};
    // b 必须仍可安全析构；可读 size()（我们保证为 0）
    assert(b.size() == 0);
    assert(c.size() == 3);

    // string 展示标准库移动
    std::string s = "payload-string-long-enough";
    std::string t = std::move(s);
    assert(t.find("payload") != std::string::npos);
    // s 有效但未指定；可赋值/析构
    s = "reuse";
    assert(s == "reuse");

    std::cout << "[expert] nothrow move; string reused after move\n";
    std::cout << "=== move_constructor: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/move_constructor", run>;

}  // namespace

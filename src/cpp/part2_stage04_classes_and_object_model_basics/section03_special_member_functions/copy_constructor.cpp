// Topic     : 拷贝构造函数
// Doc       : 第2部分-阶段4 · 步骤 3.1
// cppreference: https://en.cppreference.com/cpp/language/copy_constructor
//
// 要点: T(const T&) 用另一对象初始化新对象；隐式版本逐成员拷贝；
//       深拷贝 vs 浅拷贝；按值传参/返回可能调用拷贝（常被 elide）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace {

struct Shallow {
    int* p = nullptr;
    // 隐式拷贝：只拷指针 → 双删风险。这里故意用 =default 并避免 double-delete：不在析构 delete。
    // 真正资源类必须深拷贝或禁拷贝。
};

class DeepBuffer {
    std::size_t n_ = 0;
    char* data_ = nullptr;

public:
    DeepBuffer() = default;

    explicit DeepBuffer(std::string_view s) : n_(s.size()), data_(new char[n_ + 1]) {
        std::memcpy(data_, s.data(), n_);
        data_[n_] = '\0';
    }

    // 深拷贝
    DeepBuffer(const DeepBuffer& other) : n_(other.n_), data_(other.data_ ? new char[n_ + 1] : nullptr) {
        if (data_) {
            std::memcpy(data_, other.data_, n_ + 1);
        }
    }

    DeepBuffer& operator=(const DeepBuffer&) = delete;  // 本文件聚焦拷贝构造

    ~DeepBuffer() { delete[] data_; }

    DeepBuffer(DeepBuffer&& other) noexcept
        : n_(std::exchange(other.n_, 0)), data_(std::exchange(other.data_, nullptr)) {}

    const char* c_str() const { return data_ ? data_ : ""; }
    std::size_t size() const { return n_; }
};

struct CountCopy {
    static inline int copies = 0;
    int v = 0;
    CountCopy() = default;
    explicit CountCopy(int x) : v(x) {}
    CountCopy(const CountCopy& o) : v(o.v) { ++copies; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [copy_constructor] ===\n";

    // -------------------------------------------------------------------------
    // §入门：隐式成员式拷贝
    // -------------------------------------------------------------------------
    struct Point {
        int x;
        int y;
    };
    Point a{1, 2};
    Point b = a;  // 拷贝构造
    assert(b.x == 1 && b.y == 2);
    b.x = 9;
    assert(a.x == 1);  // 独立对象
    std::cout << "[intro] Point copy independent a.x=" << a.x << '\n';

    // -------------------------------------------------------------------------
    // §进阶：深拷贝资源
    // -------------------------------------------------------------------------
    DeepBuffer s{"hello"};
    DeepBuffer t{s};  // 深拷贝
    assert(t.size() == 5);
    assert(std::string(t.c_str()) == "hello");
    assert(s.c_str() != t.c_str());  // 不同缓冲
    std::cout << "[advanced] deep copy distinct buffers\n";

    // -------------------------------------------------------------------------
    // §专家：初始化语境触发拷贝构造
    // -------------------------------------------------------------------------
    CountCopy::copies = 0;
    CountCopy c1{3};
    CountCopy c2{c1};  // 直接初始化，调用拷贝构造
    assert(CountCopy::copies == 1);
    assert(c2.v == 3);

    CountCopy c3 = c2;  // 拷贝初始化
    assert(CountCopy::copies == 2);

    // 声明了拷贝构造会抑制隐式移动（见 suppression_rules / rule_of_five）
    std::cout << "[expert] copy ctors fired=" << CountCopy::copies << '\n';
    std::cout << "=== copy_constructor: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/copy_constructor", run>;

}  // namespace

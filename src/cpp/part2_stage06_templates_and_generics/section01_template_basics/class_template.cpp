// Topic    : 类模板 —— 每个 T 独立实例化、成员按需生成
// Doc      : 第2部分-阶段6 · 步骤 2.1
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : class_template
// Topic id : part2/stage06/section01/class_template
// Refs     : https://en.cppreference.com/w/cpp/language/class_template
//            ISO [temp.class]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// §入门 — MiniStack<T>：类蓝图，实例化出互不相干的类型
// ---------------------------------------------------------------------------

template <typename T>
class MiniStack {
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t cap_ = 0;

    void grow() {
        const std::size_t ncap = cap_ == 0 ? 4 : cap_ * 2;
        T* nd = new T[ncap];
        for (std::size_t i = 0; i < size_; ++i) {
            nd[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = nd;
        cap_ = ncap;
    }

public:
    MiniStack() = default;
    ~MiniStack() { delete[] data_; }

    MiniStack(const MiniStack& o) : size_(o.size_), cap_(o.cap_) {
        if (cap_ == 0) {
            return;
        }
        data_ = new T[cap_];
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = o.data_[i];
        }
    }

    MiniStack& operator=(const MiniStack& o) {
        if (this == &o) {
            return *this;
        }
        MiniStack tmp(o);
        swap(tmp);
        return *this;
    }

    MiniStack(MiniStack&& o) noexcept
        : data_(std::exchange(o.data_, nullptr)), size_(std::exchange(o.size_, 0)), cap_(std::exchange(o.cap_, 0)) {}

    MiniStack& operator=(MiniStack&& o) noexcept {
        if (this == &o) {
            return *this;
        }
        delete[] data_;
        data_ = std::exchange(o.data_, nullptr);
        size_ = std::exchange(o.size_, 0);
        cap_ = std::exchange(o.cap_, 0);
        return *this;
    }

    void swap(MiniStack& o) noexcept {
        std::swap(data_, o.data_);
        std::swap(size_, o.size_);
        std::swap(cap_, o.cap_);
    }

    void push(const T& v) {
        if (size_ == cap_) {
            grow();
        }
        data_[size_++] = v;
    }

    void push(T&& v) {
        if (size_ == cap_) {
            grow();
        }
        data_[size_++] = std::move(v);
    }

    T pop() {
        if (size_ == 0) {
            throw std::out_of_range("MiniStack::pop empty");
        }
        return std::move(data_[--size_]);
    }

    const T& top() const {
        if (size_ == 0) {
            throw std::out_of_range("MiniStack::top empty");
        }
        return data_[size_ - 1];
    }

    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }
};

// ---------------------------------------------------------------------------
// §进阶 — 成员按需实例化：未调用的成员即使对 T 非法也不触发
// ---------------------------------------------------------------------------

template <typename T>
struct LazyOps {
    // 只有真正调用时才会实例化；对没有 / 的类型，只要不调用 divide 就没事
    T divide(T a, T b) const { return a / b; }
    T add(T a, T b) const { return a + b; }
};

struct Token {
    std::string text;
    Token operator+(const Token& o) const { return Token{text + o.text}; }
    // 故意没有 operator/
};

// ---------------------------------------------------------------------------
// §专家 — 两个实例是完全不同类型；定义通常放头文件
// ---------------------------------------------------------------------------

template <typename T>
struct TypeTag {
    using type = T;
    static constexpr const char* name() { return "generic"; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [class_template] 入门：MiniStack 实例化 ===\n";
    MiniStack<int> si;
    si.push(1);
    si.push(2);
    assert(si.size() == 2 && si.top() == 2);
    assert(si.pop() == 2);
    assert(si.pop() == 1);
    assert(si.empty());

    MiniStack<std::string> ss;
    ss.push("hello");
    ss.push(std::string{"world"});
    assert(ss.top() == "world");
    assert(ss.pop() == "world");
    std::cout << "MiniStack<int> / MiniStack<string> 各自独立 OK\n";

    // MiniStack<int> 与 MiniStack<double> 是两个完全独立的类
    static_assert(!std::is_same_v<MiniStack<int>, MiniStack<double>>);

    std::cout << "=== 进阶：成员按需实例化 ===\n";
    [[maybe_unused]] LazyOps<int> li;
    assert(li.divide(10, 2) == 5);
    LazyOps<Token> lt;
    Token joined = lt.add(Token{"ab"}, Token{"cd"});
    assert(joined.text == "abcd");
    // 不调用 lt.divide → 即便 Token 没有 / 也不报错
    std::cout << "Token add without instantiating divide OK\n";

    std::cout << "=== 专家：空栈异常 + 拷贝独立性 ===\n";
    MiniStack<int> a;
    a.push(42);
    MiniStack<int> b = a;
    assert(b.pop() == 42);
    assert(a.top() == 42);  // 深拷贝，互不影响
    [[maybe_unused]] bool threw = false;
    try {
        MiniStack<int>{}.pop();
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
    (void)TypeTag<int>::name();
    std::cout << "=== class_template: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/class_template", run>;

}  // namespace

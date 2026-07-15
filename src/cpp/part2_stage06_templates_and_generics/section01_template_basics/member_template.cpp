// Topic    : 成员模板 —— 类内外双层 template、转换构造
// Doc      : 第2部分-阶段6 · 步骤 2.2
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : member_template
// Topic id : part2/stage06/section01/member_template
// Refs     : https://en.cppreference.com/w/cpp/language/member_template
//            ISO [temp.mem]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 非模板类也可以有成员函数模板
// ---------------------------------------------------------------------------

struct Logger {
    // 成员函数模板：U 与 Logger 无关
    template <typename U>
    void log(const U& value) const {
        std::cout << "  [log] " << value << '\n';
    }
};

// ---------------------------------------------------------------------------
// §进阶 — 类模板 + 成员模板：从兼容类型的 Box 转换
// ---------------------------------------------------------------------------

template <typename T>
class Box {
    T value_{};

public:
    Box() = default;
    explicit Box(T v) : value_(std::move(v)) {}

    const T& get() const { return value_; }
    T& get() { return value_; }

    // 成员模板：允许 Box<U> → Box<T>（U 可转 T 时）
    template <typename U>
    explicit Box(const Box<U>& other) : value_(static_cast<T>(other.get())) {}

    template <typename U>
    Box& assign_from(const Box<U>& other);

    // 范围填充：STL 风格的迭代器成员模板（vector 范围构造同理）
    template <typename It>
    void assign_range(It first, It last) {
        // 对算术 T：取最后一个；演示成员模板参数独立于 T
        if (first == last) {
            value_ = T{};
            return;
        }
        It it = first;
        while (std::next(it) != last) {
            ++it;
        }
        value_ = static_cast<T>(*it);
    }
};

// 类外定义成员模板：两层 template 头（在类内声明，类外定义）
template <typename T>
template <typename U>
Box<T>& Box<T>::assign_from(const Box<U>& other) {
    value_ = static_cast<T>(other.get());
    return *this;
}

// 自由函数工厂：单层可变模板参数即可
template <typename T, typename U>
Box<T> make_converted(const Box<U>& src) {
    return Box<T>(src);
}

// ---------------------------------------------------------------------------
// §专家 — 成员模板不抑制隐式拷贝；转换要显式；依赖名 .template
// ---------------------------------------------------------------------------

template <typename T>
struct Holder {
    T* ptr = nullptr;

    template <typename U>
    void rebind(U* p) {
        ptr = static_cast<T*>(static_cast<void*>(p));
    }

    // 调用「依赖」成员模板时，需要 .template 消歧义
    template <typename U>
    void clear_other(Holder<U>& other) {
        // other.rebind 在依赖上下文中，解析器默认把 < 当成小于号
        other.template rebind<T>(nullptr);
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [member_template] 入门：非模板类的成员模板 ===\n";
    Logger lg;
    lg.log(42);
    lg.log(std::string{"hello"});
    lg.log(3.14);

    std::cout << "=== 进阶：Box 转换 + 范围成员模板 ===\n";
    Box<int> bi{100};
    Box<long> bl{bi};  // 成员模板转换构造 int → long
    assert(bl.get() == 100L);

    Box<double> bd;
    bd.assign_from(bi);
    assert(bd.get() == 100.0);

    Box<int> from_range;
    std::vector<int> seq{1, 2, 3, 9};
    from_range.assign_range(seq.begin(), seq.end());
    assert(from_range.get() == 9);

    auto converted = make_converted<double>(bi);
    assert(converted.get() == 100.0);
    std::cout << "Box convert int→long/double OK\n";

    std::cout << "=== 专家：成员模板 ≠ 拷贝构造；.template 消歧义 ===\n";
    // 同类型拷贝走隐式拷贝构造，不是成员模板
    Box<int> copy = bi;
    assert(copy.get() == 100);
    static_assert(std::is_copy_constructible_v<Box<int>>);

    int storage = 7;
    Holder<int> hi;
    hi.rebind(&storage);
    assert(hi.ptr == &storage);
    Holder<int> other;
    other.rebind(&storage);
    hi.clear_other(other);  // 走 .template rebind
    assert(other.ptr == nullptr);
    std::cout << "=== member_template: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/member_template", run>;

}  // namespace

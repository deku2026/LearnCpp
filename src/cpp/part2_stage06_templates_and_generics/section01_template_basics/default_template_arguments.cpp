// Topic    : 默认模板参数 —— 类型/NTTP/模板模板默认值
// Doc      : 第2部分-阶段6 · 步骤 3.5
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : default_template_arguments
// Topic id : part2/stage06/section01/default_template_arguments
// Refs     : https://en.cppreference.com/w/cpp/language/template_parameters
//            ISO [temp.param]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 类模板默认参数：像 vector 的 Allocator
// ---------------------------------------------------------------------------

template <typename T, typename Container = std::vector<T>>
class StackOf {
    Container data_;

public:
    void push(const T& v) { data_.push_back(v); }
    T pop() {
        T v = data_.back();
        data_.pop_back();
        return v;
    }
    std::size_t size() const { return data_.size(); }
    using container_type = Container;
};

// ---------------------------------------------------------------------------
// §进阶 — 函数模板默认参数；依赖前一参数
// ---------------------------------------------------------------------------

// 默认参数可以依赖前面的模板参数
template <typename T, typename U = T>
std::pair<T, U> make_pair_like(T a, U b = U{}) {
    return {a, b};
}

// 函数模板：Compare 默认 less
template <typename T, typename Compare = std::less<T>>
const T& clamp_max(const T& a, const T& b, Compare cmp = Compare{}) {
    return cmp(a, b) ? b : a;
}

// NTTP 默认值
template <typename T, std::size_t Cap = 8>
struct SmallBuf {
    T data[Cap]{};
    static constexpr std::size_t capacity = Cap;
};

// ---------------------------------------------------------------------------
// §专家 — 默认参数与偏特化/别名；覆盖默认；智能指针 Deleter
// ---------------------------------------------------------------------------

template <typename T, typename Deleter = std::default_delete<T>>
struct UniqueLite {
    T* ptr = nullptr;
    Deleter del{};

    explicit UniqueLite(T* p = nullptr) : ptr(p) {}
    ~UniqueLite() {
        if (ptr) {
            del(ptr);
        }
    }
    UniqueLite(const UniqueLite&) = delete;
    UniqueLite& operator=(const UniqueLite&) = delete;
    UniqueLite(UniqueLite&& o) noexcept : ptr(o.ptr), del(std::move(o.del)) { o.ptr = nullptr; }
    T* get() const { return ptr; }
};

struct CountingDelete {
    static inline int count = 0;
    template <typename T>
    void operator()(T* p) const {
        ++count;
        delete p;
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [default_template_arguments] 入门：默认容器 ===\n";
    StackOf<int> s;  // Container = vector<int>
    s.push(1);
    s.push(2);
    assert(s.size() == 2 && s.pop() == 2);
    static_assert(std::is_same_v<StackOf<int>::container_type, std::vector<int>>);

    StackOf<int, std::deque<int>> sd;
    sd.push(9);
    assert(sd.pop() == 9);
    std::cout << "default vector / explicit deque OK\n";

    std::cout << "=== 进阶：函数默认 / 依赖默认 / NTTP 默认 ===\n";
    auto p1 = make_pair_like(3);  // U 默认 = T = int，b 默认 0
    assert(p1.first == 3 && p1.second == 0);
    auto p2 = make_pair_like(1, std::string{"x"});
    assert(p2.second == "x");
    assert(clamp_max(3, 5) == 5);
    assert(clamp_max(3, 5, std::greater<int>{}) == 3);

    SmallBuf<char> def;
    SmallBuf<char, 16> big;
    static_assert(def.capacity == 8);
    static_assert(big.capacity == 16);
    std::cout << "defaults for pair/compare/NTTP OK\n";

    std::cout << "=== 专家：覆盖 Deleter 默认 ===\n";
    CountingDelete::count = 0;
    {
        UniqueLite<int, CountingDelete> u{new int(42)};
        assert(u.get() != nullptr && *u.get() == 42);
    }
    assert(CountingDelete::count == 1);
    // 默认 Deleter = default_delete，行为等同 unique_ptr 默认
    {
        UniqueLite<int> plain{new int(1)};
        assert(*plain.get() == 1);
    }
    std::cout << "=== default_template_arguments: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/default_template_arguments", run>;

}  // namespace

// Topic    : 模板模板参数 —— 把「容器模板」本身当作参数
// Doc      : 第2部分-阶段6 · 步骤 3.4
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : template_template_parameter
// Topic id : part2/stage06/section01/template_template_parameter
// Refs     : https://en.cppreference.com/w/cpp/language/template_parameters
//            ISO [temp.param]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — Stack 底层存储策略由调用方选择
// ---------------------------------------------------------------------------

template <typename T, template <typename...> class Container>
class PolicyStack {
    Container<T> data_;

public:
    void push(const T& v) { data_.push_back(v); }
    void pop() {
        if (!data_.empty()) {
            data_.pop_back();
        }
    }
    const T& top() const { return data_.back(); }
    bool empty() const { return data_.empty(); }
    auto size() const { return data_.size(); }
};

// ---------------------------------------------------------------------------
// §进阶 — 适配器：对任意「单类型参数模板」做包装
// ---------------------------------------------------------------------------

template <template <typename> class Box, typename T>
struct Nested {
    Box<T> inner;
};

// 简易单参数模板，用于 Nested
template <typename T>
struct Opt {
    T value{};
    bool has = false;
    void set(T v) {
        value = v;
        has = true;
    }
};

// 用模板模板参数写「把 F 应用到类型」的元函数壳
template <template <typename> class F, typename T>
using apply_t = F<T>;

// ---------------------------------------------------------------------------
// §专家 — typename... 匹配多参数模板（vector 有 Allocator）
// ---------------------------------------------------------------------------
// std::vector 实际是 template<class T, class Alloc = allocator<T>>
// 用 template<typename...> class 才能匹配默认参数模板。

template <template <typename...> class Seq>
struct SequenceTraits {
    template <typename T>
    using type = Seq<T>;

    template <typename T>
    static constexpr bool is_vector_like = std::is_same_v<Seq<T>, std::vector<T>>;
};

template <typename T, template <typename...> class C1, template <typename...> class C2>
bool same_size_policy(const C1<T>& a, const C2<T>& b) {
    return a.size() == b.size();
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [template_template_parameter] 入门：策略栈 ===\n";
    PolicyStack<int, std::vector> sv;
    sv.push(1);
    sv.push(2);
    assert(sv.size() == 2 && sv.top() == 2);

    PolicyStack<int, std::deque> sd;
    sd.push(10);
    assert(sd.top() == 10);

    PolicyStack<int, std::list> sl;
    sl.push(7);
    sl.push(8);
    assert(sl.size() == 2);
    std::cout << "vector/deque/list backends OK\n";

    std::cout << "=== 进阶：单参数模板模板 + apply_t ===\n";
    Nested<Opt, int> n;
    n.inner.set(42);
    assert(n.inner.has && n.inner.value == 42);
    using O = apply_t<Opt, double>;
    O o;
    o.set(1.5);
    assert(o.value == 1.5);
    std::cout << "Nested Opt + apply_t OK\n";

    std::cout << "=== 专家：匹配带默认实参的标准容器 ===\n";
    static_assert(SequenceTraits<std::vector>::is_vector_like<int>);
    static_assert(!SequenceTraits<std::deque>::is_vector_like<int>);
    std::vector<int> v{1, 2, 3};
    std::deque<int> d{4, 5, 6};
    assert(same_size_policy(v, d));
    // 注意：template template 参数匹配规则在 C++17 放宽（参数默认值兼容）
    std::cout << "=== template_template_parameter: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/template_template_parameter", run>;

}  // namespace

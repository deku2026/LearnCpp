// Topic     : explicit 构造函数
// Doc       : 第2部分-阶段4 · 步骤 1.4
// cppreference: https://en.cppreference.com/cpp/language/explicit
//
// 要点: 单参（或除首参外均有默认值）构造可参与隐式转换；explicit 禁止该路径；
//       值类型单参构造默认加 explicit；C++20 起 explicit(bool) 条件显式。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

struct Buffer {
    explicit Buffer(int size) : size_(size) { assert(size_ >= 0); }
    int size() const { return size_; }

private:
    int size_;
};

struct Loose {
    Loose(int size) : size_(size) {}  // 非 explicit：允许 int → Loose 隐式转换
    int size() const { return size_; }

private:
    int size_;
};

void use_buf(Buffer b) {
    (void)b;
}
void use_loose(Loose l) {
    (void)l;
}

// 多参数 + 默认实参 → 仍可单参调用，建议 explicit
struct Path {
    explicit Path(std::string p, bool /*normalize*/ = true) : path_(std::move(p)) {}
    const std::string& str() const { return path_; }

private:
    std::string path_;
};

// C++20: explicit(bool) —— 按条件决定是否允许隐式转换
template <class T>
struct Wrapper {
    T value;
    explicit(!std::is_arithmetic_v<T>) Wrapper(T v) : value(std::move(v)) {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [explicit_constructor] ===\n";

    // -------------------------------------------------------------------------
    // §入门：禁止意外的 int → Buffer
    // -------------------------------------------------------------------------
    // use_buf(10);                 // ❌ explicit 禁止
    use_buf(Buffer{10});  // ✅ 显式构造
    use_buf(Buffer(10));
    Buffer b{32};
    assert(b.size() == 32);
    std::cout << "[intro] explicit Buffer size=" << b.size() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：非 explicit 的隐式转换风险
    // -------------------------------------------------------------------------
    use_loose(10);  // ⚠️ 编译通过：10 → Loose —— 可能掩盖 API 误用
    Loose l = 5;    // 拷贝初始化也走隐式转换
    assert(l.size() == 5);

    // vector 经典坑：想要「10 个元素」却写成需要转换的上下文时，explicit 能救命
    std::vector<int> v(10);  // 大小构造，不是 initializer_list
    assert(v.size() == 10);
    std::cout << "[advanced] Loose allows implicit; vector size ctor n=" << v.size() << '\n';

    // -------------------------------------------------------------------------
    // §专家：默认实参单参路径 + explicit(bool)
    // -------------------------------------------------------------------------
    Path p{"/tmp/x"};
    assert(p.str() == "/tmp/x");
    // Path p2 = std::string("/a"); // ❌ explicit 禁止拷贝初始化隐式转换

    Wrapper<int> wi{3};  // arithmetic → explicit(false) → 可隐式
    Wrapper<int> wi2 = 4;
    assert(wi.value == 3 && wi2.value == 4);

    Wrapper<std::string> ws{std::string{"hi"}};  // 非算术 → explicit(true)
    // Wrapper<std::string> ws2 = std::string{"x"}; // ❌
    assert(ws.value == "hi");

    std::cout << "[expert] explicit(bool) arithmetic free, string locked; wi2=" << wi2.value << '\n';
    std::cout << "=== explicit_constructor: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/explicit_constructor", run>;

}  // namespace

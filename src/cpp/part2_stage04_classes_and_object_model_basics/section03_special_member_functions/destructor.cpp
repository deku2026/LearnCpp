// Topic     : 析构函数
// Doc       : 第2部分-阶段4 · 步骤 3.1 / 2
// cppreference: https://en.cppreference.com/cpp/language/destructor
//
// 要点: 对象生命周期结束时调用；成员/基类按构造逆序析构；析构中勿抛异常；
//       用户声明析构会抑制隐式移动（见 suppression_rules）。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

std::string g_log;

struct Tracer {
    std::string name;
    explicit Tracer(std::string n) : name(std::move(n)) { g_log += '+' + name; }
    ~Tracer() { g_log += '-' + name; }
};

class Holder {
    Tracer a_{"A"};
    Tracer b_{"B"};

public:
    Holder() = default;
    // 用户声明析构：即使空，也算「用户声明」→ 抑制隐式移动
    ~Holder() { g_log += "~H"; }
};

class NoThrowDtor {
    int* p_;

public:
    explicit NoThrowDtor(int v) : p_(new int(v)) {}
    ~NoThrowDtor() noexcept { delete p_; }  // 惯例：析构 noexcept
    int get() const { return *p_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [destructor] ===\n";

    // -------------------------------------------------------------------------
    // §入门：作用域结束调用析构
    // -------------------------------------------------------------------------
    g_log.clear();
    {
        Tracer t{"T"};
        assert(g_log == "+T");
    }
    assert(g_log == "+T-T");
    std::cout << "[intro] log=" << g_log << '\n';

    // -------------------------------------------------------------------------
    // §进阶：成员逆序析构 + 用户析构体
    // -------------------------------------------------------------------------
    g_log.clear();
    {
        Holder h;
        // 构造：A then B；析构：~H then ~B then ~A（成员逆序，类析构体先执行）
    }
    assert(g_log == "+A+B~H-B-A");
    std::cout << "[advanced] Holder order=" << g_log << '\n';

    // -------------------------------------------------------------------------
    // §专家：容器元素析构、noexcept
    // -------------------------------------------------------------------------
    g_log.clear();
    {
        std::vector<Tracer> v;
        v.emplace_back("X");
        v.emplace_back("Y");
    }
    // 析构顺序：后进先出（Y then X）—— vector 逆序销毁元素
    assert(g_log.find("+X") != std::string::npos);
    assert(g_log.find("-Y") != std::string::npos);
    assert(g_log.find("-X") != std::string::npos);

    NoThrowDtor n{99};
    assert(n.get() == 99);
    static_assert(noexcept(std::declval<NoThrowDtor>().~NoThrowDtor()));

    // 多态基类需要 virtual 析构 —— 见 section08/virtual_destructor
    std::cout << "[expert] vector element dtors + noexcept dtor ok\n";
    std::cout << "=== destructor: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/destructor", run>;

}  // namespace

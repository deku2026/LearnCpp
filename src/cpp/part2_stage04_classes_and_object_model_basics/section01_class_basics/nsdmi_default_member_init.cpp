// Topic     : NSDMI（默认成员初始化）
// Doc       : 第2部分-阶段4 · 步骤 1.3
// cppreference: https://en.cppreference.com/cpp/language/data_members#Member_initialization
//
// 要点: 在声明处给非静态成员默认值；未出现在构造初始化列表中的成员用 NSDMI；
//       初始化列表中的显式初始化覆盖 NSDMI；与 =default 构造搭配极佳。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

class Config {
    int timeout_ = 30;  // NSDMI
    bool verbose_ = false;
    std::string path_ = "/tmp";

public:
    Config() = default;  // 全部走 NSDMI

    explicit Config(int t) : timeout_(t) {}  // 只覆盖 timeout_，其余仍 NSDMI

    Config(int t, std::string p) : timeout_(t), path_(std::move(p)) {
        // verbose_ 仍用 NSDMI false
    }

    int timeout() const { return timeout_; }
    bool verbose() const { return verbose_; }
    const std::string& path() const { return path_; }
};

// bit-field / 数组也可 NSDMI（C++11+）
struct Flags {
    unsigned ready : 1 = 0;
    unsigned error : 1 = 0;
    int values[3] = {1, 2, 3};
};

// 若成员无默认构造且无 NSDMI，则必须在每个构造函数里初始化它
struct NeedsInit {
    // const 成员：必须初始化列表或 NSDMI
    const int id_ = -1;
    explicit NeedsInit(int id) : id_(id) {}  // 覆盖 NSDMI
    NeedsInit() = default;                   // 使用 id_ = -1
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [nsdmi_default_member_init] ===\n";

    // -------------------------------------------------------------------------
    // §入门：默认构造全用 NSDMI
    // -------------------------------------------------------------------------
    Config c0;
    assert(c0.timeout() == 30);
    assert(!c0.verbose());
    assert(c0.path() == "/tmp");
    std::cout << "[intro] default Config timeout=" << c0.timeout() << " path=" << c0.path() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：列表覆盖部分成员
    // -------------------------------------------------------------------------
    Config c1{60};
    assert(c1.timeout() == 60);
    assert(c1.path() == "/tmp");  // 未覆盖 → NSDMI

    Config c2{10, "/var/log"};
    assert(c2.timeout() == 10 && c2.path() == "/var/log");
    assert(!c2.verbose());
    std::cout << "[advanced] partial override path=" << c2.path() << '\n';

    // -------------------------------------------------------------------------
    // §专家：优先级与 const / 数组
    // -------------------------------------------------------------------------
    [[maybe_unused]] Flags f;
    assert(f.ready == 0 && f.error == 0);
    assert(f.values[0] == 1 && f.values[2] == 3);

    [[maybe_unused]] NeedsInit n0;
    assert(n0.id_ == -1);
    NeedsInit n1{42};
    assert(n1.id_ == 42);

    // 优先级：构造初始化列表 > NSDMI > （否则默认初始化/未初始化）
    // 注意：体中赋值发生在「成员已按列表/NSDMI 初始化之后」
    std::cout << "[expert] Flags/NeedsInit NSDMI ok; id=" << n1.id_ << '\n';
    std::cout << "=== nsdmi_default_member_init: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/nsdmi_default_member_init", run>;

}  // namespace

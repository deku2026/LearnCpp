// Topic     : 基本 I/O（<iostream>）
// Doc       : 第2部分-阶段1 · 步骤 8
// cppreference: https://en.cppreference.com/cpp/io/cout
//               https://en.cppreference.com/cpp/header/iostream
//               https://en.cppreference.com/cpp/io/manip/endl
//
// 要点: cout/cin/cerr/clog；<< / >> 类型安全与链式；endl vs '\\n'；
//       ios_base::Init 静态初始化代价；C++20/23 format/print 预告。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <sstream>
#include <string>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<print>)
#include <print>
#endif
#endif

namespace {

// 用 stringstream 做可断言的"流教室"，避免 topic 运行时阻塞等待真实 stdin。
void demo_extract_insert() {
    std::istringstream in{"Ada 36"};
    std::string name;
    int age = 0;
    in >> name >> age;  // 按空白分隔；类型安全解析 int
    assert(static_cast<bool>(in));
    assert(name == "Ada");
    assert(age == 36);

    std::ostringstream out;
    out << "Hi " << name << ", age " << age << '\n';  // 链式：返回 ostream&
    assert(out.str() == "Hi Ada, age 36\n");
}

void demo_failbit_on_mismatch() {
    std::istringstream in{"not-a-number"};
    int value = -1;
    in >> value;
    assert(!in);  // 提取失败 → failbit（可移植）
    // 失败时目标对象是否保持原值：实现可不同（有的保持 -1，有的写成 0）。
    // 标准保证“失败不抛则流状态可见”；不要依赖 value 的具体残留。
    (void)value;
    assert(in.fail());
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iostream_basic_io] ===\n";

    // -------------------------------------------------------------------------
    // §入门：四个标准流
    // -------------------------------------------------------------------------
    // std::cout  — 标准输出（缓冲）
    // std::cin   — 标准输入（缓冲）
    // std::cerr  — 标准错误（默认不缓冲，立即可见）
    // std::clog  — 日志/诊断（写 stderr，但有缓冲）
    std::cout << "[intro] stdout via std::cout\n";
    std::cerr << "[intro] stderr via std::cerr (typically unbuffered)\n";
    std::clog << "[intro] stderr via std::clog (buffered diagnostic)\n";

    demo_extract_insert();
    std::cout << "[intro] << inserts, >> extracts; both are overloaded & chainable\n";

    // -------------------------------------------------------------------------
    // §进阶：endl vs '\\n'；格式状态；错误状态
    // -------------------------------------------------------------------------
    // std::endl = 输出 '\\n' + flush。高频日志里 endl 会拖慢 I/O。
    // 默认用 '\\n'；交互提示或崩溃前日志再 flush / endl。
    std::cout << "[tip] prefer '\\n'; use endl only when you must flush\n";
    std::cout << "[tip] explicit flush without newline: " << std::flush;

    demo_failbit_on_mismatch();
    std::cout << "[advanced] failed extraction sets failbit; check stream state\n";

    // boolalpha 等操纵符改变后续解析/打印方式（状态粘在流上）
    {
        std::ostringstream os;
        os << std::boolalpha << true << ' ' << false;
        assert(os.str() == "true false");
    }

    // -------------------------------------------------------------------------
    // §专家：ios_base::Init 与现代替代
    // -------------------------------------------------------------------------
    // 每个 #include <iostream> 的 TU 如同拥有一个静态 std::ios_base::Init；
    // 其构造在 main 前初始化标准流——所以进 main 就能用 cout。
    // 代价: 即使你不用流，include 仍可能引入静态初始化开销（嵌入式/极致启动敏感）。
    //
    // 现代格式化（阶段 10 深讲）:
    //   C++20 std::format
    //   C++23 std::print / std::println  （新一代 Hello World）
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L && __has_include(<print>)
    std::println("[expert] std::println available (C++23 <print>)");
#else
    std::cout << "[expert] <print>/std::println not available; iostream fallback OK\n";
#endif

    // 同步: 默认 std::ios_base::sync_with_stdio(true) 与 C stdio 同步，便于混用 printf。
    // 纯 C++ 高性能路径可 sync_with_stdio(false)，但本教学程序保持默认。
    assert(std::cout.good());

    std::cout << "=== iostream_basic_io: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/iostream_basic_io", run>;

}  // namespace

// Topic    : iostream 体系总览（cin/cout/cerr/clog + 自定义类型流插入）
// Doc      : 第3部分-标准库系统化.md · 库 5.1
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_overview
// Topic id : part3/section05/iostream_overview
// Refs     : https://en.cppreference.com/w/cpp/io
//            https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace {

struct Point {
    int x{};
    int y{};
};

// 自定义类型：非成员 operator<< / >> —— 让类型“流化”
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << '(' << p.x << ", " << p.y << ')';
}

std::istream& operator>>(std::istream& is, Point& p) {
    // 简化：直接读两个 int；真实协议常要解析括号/逗号
    return is >> p.x >> p.y;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iostream_overview] 标准流角色 ===\n";
    // cout: 缓冲标准输出；cerr: 无缓冲错误；clog: 缓冲日志
    std::cout << "stdout via cout\n";
    std::cerr << "stderr via cerr (unbuffered)\n";
    std::clog << "log via clog (buffered)\n";

    std::cout << "=== 插入/提取（用 stringstream 可测） ===\n";
    {
        std::ostringstream oss;
        oss << "name=" << "Ada" << " age=" << 36;
        assert(oss.str() == "name=Ada age=36");
        std::cout << "ostringstream: " << oss.str() << '\n';

        std::istringstream iss("Ada 36");
        std::string name;
        int age = 0;
        iss >> name >> age;
        assert(name == "Ada" && age == 36);
        std::cout << "istringstream: name=" << name << " age=" << age << '\n';
    }

    std::cout << "=== 自定义类型 operator<< / >> ===\n";
    {
        Point p{3, 4};
        std::ostringstream oss;
        oss << p;
        assert(oss.str() == "(3, 4)");
        std::cout << "Point out: " << oss.str() << '\n';

        std::istringstream iss("10 20");
        Point q{};
        iss >> q;
        assert(q.x == 10 && q.y == 20);
        std::cout << "Point in: " << q << '\n';
    }

    std::cout << "=== 与 C++23 print 的分工 ===\n";
    // 新代码格式化输出优先 std::print/format；iostream 擅长流式读写、状态机、自定义类型协议
    std::cout << "prefer std::print for format; iostream for streaming IO\n";

    std::cout << "[iostream_overview] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/iostream_overview", run>;

}  // namespace

// Topic    : stringstream 内存字符串流
// Doc      : 第3部分-标准库系统化.md · 库 5.2
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : sstream_overview
// Topic id : part3/section05/sstream_overview
// Refs     : https://en.cppreference.com/w/cpp/io/basic_stringstream
//            https://en.cppreference.com/w/cpp/header/sstream

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [sstream_overview] ostringstream 拼接 ===\n";
    {
        std::ostringstream oss;
        oss << "x=" << 42 << ", y=" << 3.14;
        assert(oss.str() == "x=42, y=3.14");
        std::cout << oss.str() << '\n';
    }

    std::cout << "=== istringstream 解析 ===\n";
    {
        std::istringstream iss("10 20 30");
        int a = 0, b = 0, c = 0;
        iss >> a >> b >> c;
        assert(a == 10 && b == 20 && c == 30);
        std::cout << "a+b+c=" << (a + b + c) << '\n';
    }

    std::cout << "=== stringstream 双向 + str()/str(s) ===\n";
    {
        std::stringstream ss;
        ss << "hello " << 1;
        assert(ss.str() == "hello 1");

        // 重置内容与状态
        ss.str("99 bottles");
        ss.clear();
        int n = 0;
        std::string word;
        ss >> n >> word;
        assert(n == 99 && word == "bottles");
        std::cout << "reset parse: " << n << ' ' << word << '\n';
    }

    std::cout << "=== 失败恢复（与 iostream 状态同一套） ===\n";
    {
        std::istringstream iss("pi=3.14");
        int n = 0;
        assert(!(iss >> n));  // 'p' 失败
        iss.clear();
        // 跳到 '=' 后
        char ch = 0;
        while (iss.get(ch) && ch != '=') {
        }
        double v = 0;
        assert(static_cast<bool>(iss >> v));
        assert(v > 3.0 && v < 3.2);
        std::cout << "recovered v=" << v << '\n';
    }

    std::cout << "note: sstream 可能多次堆分配；高性能场景看 spanstream\n";
    std::cout << "[sstream_overview] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/sstream_overview", run>;

}  // namespace

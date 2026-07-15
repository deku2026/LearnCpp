// Topic    : iostream manipulators（iomanip 粘性状态坑）
// Doc      : 第3部分-标准库系统化.md · 库 5.1 manipulator
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_manipulators
// Topic id : part3/section05/iostream_manipulators
// Refs     : https://en.cppreference.com/w/cpp/io/manip
//            https://en.cppreference.com/w/cpp/header/iomanip

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iostream_manipulators] 进制 / 填充 / 宽度 ===\n";
    {
        std::ostringstream oss;
        oss << std::hex << std::showbase << 255;
        assert(oss.str() == "0xff");
        std::cout << "hex showbase: " << oss.str() << '\n';

        // ⚠️ hex 是粘性的，必须改回 dec
        oss.str({});
        oss.clear();
        oss << 10;  // 仍是 hex → "a"
        assert(oss.str() == "a");
        std::cout << "sticky hex still on: " << oss.str() << '\n';

        oss.str({});
        oss.clear();
        oss << std::dec << 10;
        assert(oss.str() == "10");
    }

    std::cout << "=== setw 不粘；setfill/precision 粘 ===\n";
    {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(5) << 42;
        assert(oss.str() == "00042");
        // setw 只影响下一次输出
        oss << '|' << 7;
        assert(oss.str() == "00042|7");
        std::cout << oss.str() << '\n';

        oss.str({});
        oss.clear();
        oss << std::fixed << std::setprecision(2) << 3.14159;
        assert(oss.str() == "3.14");
        // setprecision + fixed 粘性：后续浮点仍两位
        oss << ' ' << 2.5;
        assert(oss.str() == "3.14 2.50");
        std::cout << "sticky precision: " << oss.str() << '\n';
    }

    std::cout << "=== left/right 对齐 ===\n";
    {
        std::ostringstream oss;
        oss << std::left << std::setfill('.') << std::setw(8) << "hi";
        assert(oss.str() == "hi......");
        std::cout << oss.str() << '\n';
    }

    std::cout << "=== boolalpha ===\n";
    {
        std::ostringstream oss;
        oss << std::boolalpha << true << ' ' << false;
        assert(oss.str() == "true false");
        std::cout << oss.str() << '\n';
    }

    std::cout << "tip: sticky manipulators are a top iostream footgun; "
                 "prefer std::format/print for one-shot formatting\n";
    std::cout << "[iostream_manipulators] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/iostream_manipulators", run>;

}  // namespace

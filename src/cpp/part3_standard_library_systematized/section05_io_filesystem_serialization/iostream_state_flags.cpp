// Topic    : 流状态位 good/eof/fail/bad + clear()
// Doc      : 第3部分-标准库系统化.md · 库 5.1 流状态
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_state_flags
// Topic id : part3/section05/iostream_state_flags
// Refs     : https://en.cppreference.com/w/cpp/io/ios_base/iostate
//            https://en.cppreference.com/w/cpp/io/basic_ios/rdstate

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void dump_state(const char* tag, const std::ios& s) {
    std::cout << tag << " good=" << s.good() << " eof=" << s.eof() << " fail=" << s.fail() << " bad=" << s.bad()
              << " rdstate=0x" << std::hex << static_cast<unsigned>(s.rdstate()) << std::dec << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iostream_state_flags] 成功读 int ===\n";
    {
        std::istringstream iss("42 rest");
        int n = 0;
        assert(static_cast<bool>(iss >> n));
        assert(n == 42);
        assert(iss.good());
        dump_state("after 42", iss);
    }

    std::cout << "=== 格式失败 failbit：必须 clear() ===\n";
    {
        std::istringstream iss("42 abc 7");
        int n = 0;
        assert(static_cast<bool>(iss >> n) && n == 42);

        // "abc" 无法解析为 int → fail
        assert(!(iss >> n));
        assert(iss.fail());
        assert(!iss.bad());  // 逻辑失败，不是底层 I/O 崩溃
        dump_state("after abc fail", iss);

        // ⚠️ 坑：fail 后后续操作全部失败，直到 clear
        assert(!(iss >> n));
        iss.clear();  // 清除 failbit
        // 还停在 'a' 处，需要跳过坏 token
        std::string junk;
        iss >> junk;
        assert(junk == "abc");
        assert(static_cast<bool>(iss >> n) && n == 7);
        dump_state("after clear + recover", iss);
        std::cout << "recovered n=" << n << '\n';
    }

    std::cout << "=== EOF：读尽后 eofbit ===\n";
    {
        std::istringstream iss("1 2");
        int a = 0;
        int b = 0;
        int c = 0;
        assert(static_cast<bool>(iss >> a >> b));
        assert(!(iss >> c));  // 没有更多数据
        assert(iss.eof() || iss.fail());
        dump_state("after EOF", iss);
    }

    std::cout << "=== while (stream >> x) 靠状态驱动 ===\n";
    {
        std::istringstream iss("3 5 8");
        int sum = 0;
        int x = 0;
        while (iss >> x) {
            sum += x;
        }
        assert(sum == 16);
        std::cout << "sum=" << sum << '\n';
    }

    std::cout << "[iostream_state_flags] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/iostream_state_flags", run>;

}  // namespace

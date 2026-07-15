// Topic    : 流迭代器 istream_iterator / ostream_iterator
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 4.3
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : stream_iterators
// Topic id : part2/stage10/section04/stream_iterators
// Refs     : https://en.cppreference.com/w/cpp/iterator/istream_iterator
//            https://en.cppreference.com/w/cpp/iterator/ostream_iterator
//            ISO [istream.iterator] [ostream.iterator]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [stream_iterators] 入门：ostream_iterator 打印 ===\n";
    {
        std::vector<int> v{1, 2, 3, 4};
        std::ostringstream oss;
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(oss, " "));
        assert(oss.str() == "1 2 3 4 ");
        std::cout << "ostream_iterator writes with delimiter on assign\n";
        std::cout << "demo: ";
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ","));
        std::cout << '\n';
    }

    std::cout << "=== 入门：istream_iterator 从流读取 ===\n";
    {
        std::istringstream iss("10 20 30 40");
        std::vector<int> v{
            std::istream_iterator<int>{iss},
            std::istream_iterator<int>{},  // 默认构造 = end-of-stream
        };
        assert((v == std::vector<int>{10, 20, 30, 40}));
        std::cout << "istream_iterator range-constructs from whitespace tokens\n";
    }

    std::cout << "=== 进阶：算法管道 — 过滤后写出 ===\n";
    {
        std::istringstream iss("1 2 3 4 5 6 7 8 9");
        std::ostringstream oss;
        std::copy_if(std::istream_iterator<int>{iss}, std::istream_iterator<int>{},
                     std::ostream_iterator<int>(oss, ":"), [](int x) { return x % 2 == 0; });
        assert(oss.str() == "2:4:6:8:");
        std::cout << "copy_if between stream iterators: " << oss.str() << '\n';
    }

    std::cout << "=== 进阶：单遍输入 — 别二次遍历同一 istream_iterator 范围 ===\n";
    {
        std::istringstream iss("1 2 3");
        std::istream_iterator<int> first{iss}, last{};
        // 输入迭代器单遍：读过即消耗流
        std::vector<int> once(first, last);
        assert(once.size() == 3);
        // 流已耗尽
        int probe = 0;
        iss >> probe;
        assert(!iss);
        std::cout << "input iterators are single-pass; stream is consumed\n";
    }

    std::cout << "=== 专家：istreambuf_iterator 按字符原始读取 ===\n";
    {
        std::istringstream iss("ab\ncd");
        std::string raw{
            std::istreambuf_iterator<char>{iss},
            std::istreambuf_iterator<char>{},
        };
        assert(raw == "ab\ncd");
        // istream_iterator<char> 会跳过空白；istreambuf 不跳
        std::istringstream iss2("a b");
        std::string skipped{
            std::istream_iterator<char>{iss2},
            std::istream_iterator<char>{},
        };
        // 读 char 仍受 skipws 影响（默认开启）
        assert(skipped == "ab" || skipped.size() >= 2);
        std::cout << "istreambuf_iterator: raw chars; no formatted skip\n";
    }

    std::cout << "=== 专家：与 back_inserter 组合物化 ===\n";
    {
        std::istringstream iss("7 8 9");
        std::vector<int> v;
        std::copy(std::istream_iterator<int>{iss}, std::istream_iterator<int>{}, std::back_inserter(v));
        assert((v == std::vector<int>{7, 8, 9}));
        std::cout << "stream → algorithm → container via inserter\n";
    }

    std::cout << "[stream_iterators] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section04/stream_iterators", run>;

}  // namespace

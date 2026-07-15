// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.5 libFuzzer
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : libfuzzer_intro
// Topic id : part4/section04/libfuzzer_intro
//
// 要点: LLVMFuzzerTestOneInput(data,size) 喂随机字节; 与 ASan 同开抓崩溃。
//       本文件模拟 harness + 变异, 不链接 libFuzzer 运行时。

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 被测解析器: 对畸形输入必须不崩
bool parse_length_prefixed(const uint8_t* data, std::size_t size, std::string& out) {
    out.clear();
    if (size < 1) {
        return false;
    }
    const std::size_t n = data[0];
    if (size < 1 + n) {
        return false;  // 不越界读
    }
    out.assign(reinterpret_cast<const char*>(data + 1), n);
    return true;
}

// libFuzzer 入口形态
extern "C" int LLVMFuzzerTestOneInput_demo(const uint8_t* data, std::size_t size) {
    std::string out;
    (void)parse_length_prefixed(data, size, out);
    return 0;  // 非 0 保留
}

// 极简变异: 翻比特 / 截断
std::vector<std::vector<uint8_t>> mutate(const std::vector<uint8_t>& seed) {
    std::vector<std::vector<uint8_t>> out;
    out.push_back(seed);
    if (!seed.empty()) {
        auto m = seed;
        m[0] ^= 0xFF;
        out.push_back(m);
        out.emplace_back(seed.begin(), seed.begin() + seed.size() / 2);
    }
    out.push_back({});  // empty
    out.push_back(std::vector<uint8_t>(64, 0x41));
    return out;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== libfuzzer_intro ===\n";

    // 合法包: len=3 "abc"
    std::vector<uint8_t> good{3, 'a', 'b', 'c'};
    std::string out;
    assert(parse_length_prefixed(good.data(), good.size(), out));
    assert(out == "abc");

    // 畸形: 声称长度过长 — 安全失败
    std::vector<uint8_t> bad{10, 'x'};
    assert(!parse_length_prefixed(bad.data(), bad.size(), out));

    // 模糊循环
    int execs = 0;
    auto corpus = mutate(good);
    for (const auto& c : corpus) {
        LLVMFuzzerTestOneInput_demo(c.data(), c.size());
        ++execs;
    }
    assert(execs >= 4);
    std::cout << "  harness execs=" << execs << " (no crash)\n";

    // 编译口令(文档):
    // clang++ -fsanitize=fuzzer,address -std=c++23 fuzz.cpp
    [[maybe_unused]] const char* sanitize = "fuzzer,address";
    assert(std::string_view(sanitize).find("fuzzer") != std::string_view::npos);
    std::cout << "  build: -fsanitize=fuzzer,address\n";

    std::cout << "libfuzzer_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/libfuzzer_intro", run>;

}  // namespace

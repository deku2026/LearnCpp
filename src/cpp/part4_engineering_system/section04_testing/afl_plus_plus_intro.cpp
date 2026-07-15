// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.5 AFL++
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : afl_plus_plus_intro
// Topic id : part4/section04/afl_plus_plus_intro
//
// 要点: 覆盖率引导模糊(灰盒); 语料目录 + 持久化模式; 与 libFuzzer 同类目标。
//       模拟边覆盖反馈选择变异种子。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace {

// 简易「边覆盖」: 根据输入走不同分支, 返回路径指纹
std::uint64_t coverage_fingerprint(const std::vector<std::uint8_t>& in) {
    std::uint64_t h = 0;
    if (in.empty()) {
        h |= 1ull << 0;
        return h;
    }
    h |= 1ull << 1;
    if (in[0] == 0xFF) {
        h |= 1ull << 2;
    }
    if (in.size() > 4) {
        h |= 1ull << 3;
    }
    if (in.size() > 1 && in[1] == 'Q') {
        h |= 1ull << 4;
    }
    // 业务分支
    int sum = 0;
    for (auto b : in) {
        sum += b;
    }
    if (sum > 200) {
        h |= 1ull << 5;
    }
    return h;
}

// 被测: 状态机式解析, 目标是稳健
enum class Status { Ok, Reject };

Status handle(const std::vector<std::uint8_t>& in) {
    if (in.size() >= 2 && in[0] == 'Q' && in[1] == 'Q') {
        return Status::Reject;  // 协议禁止
    }
    return Status::Ok;
}

struct AflQueue {
    std::vector<std::vector<std::uint8_t>> seeds;
    std::set<std::uint64_t> seen_cov;
};

bool offer(AflQueue& q, std::vector<std::uint8_t> input) {
    const auto cov = coverage_fingerprint(input);
    if (q.seen_cov.insert(cov).second) {
        q.seeds.push_back(std::move(input));
        return true;  // 新覆盖 → 入队
    }
    return false;
}

std::vector<std::uint8_t> mutate_flip(std::vector<std::uint8_t> s, std::size_t i) {
    if (i < s.size()) {
        s[i] ^= 0x01;
    }
    return s;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== afl_plus_plus_intro (coverage-guided sketch) ===\n";

    AflQueue q;
    // 初始语料
    assert(offer(q, {}));
    assert(offer(q, {'A'}));
    assert(offer(q, {0xFF, 'x'}));
    const auto initial = q.seeds.size();
    assert(initial >= 3);

    // 变异: 只保留增加覆盖的
    int new_cov = 0;
    auto base = q.seeds;
    for (const auto& s : base) {
        for (std::size_t i = 0; i < std::max<std::size_t>(s.size(), 1); ++i) {
            auto m = mutate_flip(s, i);
            if (offer(q, std::move(m))) {
                ++new_cov;
            }
            auto long_in = s;
            long_in.insert(long_in.end(), 8, 0x10);
            if (offer(q, std::move(long_in))) {
                ++new_cov;
            }
        }
    }
    std::cout << "  seeds=" << q.seeds.size() << " new_cov_mutations=" << new_cov << " unique_cov=" << q.seen_cov.size()
              << '\n';
    assert(q.seen_cov.size() >= initial);

    // 所有种子不崩溃
    for (const auto& s : q.seeds) {
        assert(handle(s) == Status::Ok || handle(s) == Status::Reject);
    }

    // AFL++ 用法心智: afl-clang-fast++ 插桩; afl-fuzz -i in -o out -- ./target
    std::cout << "  workflow: instrument -> afl-fuzz -i corpus -o findings\n";
    std::cout << "afl_plus_plus_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/afl_plus_plus_intro", run>;

}  // namespace

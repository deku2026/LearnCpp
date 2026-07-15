// Topic    : 容器选择 = 大 O + 缓存局部性（vector 默认优先）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 1.4（验收点）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : container_choice_cache_complexity
// Topic id : part2/stage10/section01/container_choice_cache_complexity
// Refs     : https://en.cppreference.com/w/cpp/container
//            Effective STL (Meyers) Item 1
//            ISO [sequences]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <string_view>
#include <vector>

namespace {

template <class F>
std::int64_t time_us(F&& f) {
    const auto t0 = std::chrono::steady_clock::now();
    f();
    const auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [container_choice_cache_complexity] 入门：选型表心智 ===\n";
    {
        // 默认：vector（连续、随机访问、尾插摊还 O(1)）
        // 固定 N：array
        // 头尾都要高效：deque
        // 已知迭代器处频繁插删且需迭代器稳定：list / forward_list
        std::array<const char*, 4> rules{
            "vector default",
            "array if N compile-time fixed",
            "deque if front+back hot",
            "list only if measured + iterator stability needed",
        };
        assert(rules.size() == 4);
        for (auto* r : rules) {
            std::cout << "  - " << r << '\n';
        }
    }

    std::cout << "=== 进阶：中间插入的大 O 不等于实测（小 N 上 vector 常赢）===\n";
    {
        constexpr int N = 5000;
        std::vector<int> v;
        v.reserve(static_cast<std::size_t>(N));
        for (int i = 0; i < N; ++i) {
            v.push_back(i);
        }

        std::list<int> lst(v.begin(), v.end());

        // 在中部插入同一批值：list 名义 O(1)/次但要 advance；vector O(n)/次
        // 这里各插 200 次，看相对量级（结果依赖机器，只作教学观察）
        constexpr int Inserts = 200;
        const auto us_vec = time_us([&] {
            std::vector<int> local = v;
            for (int i = 0; i < Inserts; ++i) {
                local.insert(local.begin() + local.size() / 2, i);
            }
            // 防止被优化掉
            assert(local.size() == v.size() + Inserts);
        });
        const auto us_list = time_us([&] {
            std::list<int> local = lst;
            for (int i = 0; i < Inserts; ++i) {
                auto it = local.begin();
                std::advance(it, static_cast<long>(local.size() / 2));
                local.insert(it, i);
            }
            assert(local.size() == lst.size() + Inserts);
        });
        std::cout << "mid-insert x" << Inserts << " on N=" << N << ": vector=" << us_vec << "us list=" << us_list
                  << "us\n";
        // 不强制谁更快（实现/机器差异），但两者都应完成
        assert(us_vec >= 0 && us_list >= 0);
    }

    std::cout << "=== 进阶：顺序遍历缓存 —— vector 通常碾压 list ===\n";
    {
        constexpr int N = 200000;
        std::vector<int> v(N);
        std::iota(v.begin(), v.end(), 0);
        std::list<int> lst(v.begin(), v.end());

        volatile std::int64_t sink = 0;
        const auto us_vec = time_us([&] {
            std::int64_t s = 0;
            for (int x : v) {
                s += x;
            }
            sink = s;
        });
        const auto us_list = time_us([&] {
            std::int64_t s = 0;
            for (int x : lst) {
                s += x;
            }
            sink = s;
        });
        (void)sink;
        std::cout << "scan N=" << N << ": vector=" << us_vec << "us list=" << us_list << "us\n";
        // 绝大多数平台 list 扫描更慢；若异常快也允许（超优化/计时噪声）
        assert(us_vec >= 0);
    }

    std::cout << "=== 专家：场景决策树（代码化）===\n";
    {
        enum class Need { FixedN, FrontBack, StableMidMutate, Default };
        auto pick = [](Need n) -> const char* {
            switch (n) {
                case Need::FixedN:
                    return "std::array";
                case Need::FrontBack:
                    return "std::deque (or vector+indices if only back)";
                case Need::StableMidMutate:
                    return "std::list / forward_list (measure first)";
                case Need::Default:
                    return "std::vector";
            }
            return "std::vector";
        };
        assert(std::string_view(pick(Need::Default)) == "std::vector");
        assert(std::string_view(pick(Need::FixedN)) == "std::array");
        std::cout << "Default→" << pick(Need::Default) << '\n';
        std::cout << "FrontBack→" << pick(Need::FrontBack) << '\n';
        std::cout << "StableMid→" << pick(Need::StableMidMutate) << '\n';
    }

    std::cout << "=== 专家：随机删除下标 — vector erase_if 仍常是正解 ===\n";
    {
        std::vector<int> v(1000);
        std::iota(v.begin(), v.end(), 0);
        std::mt19937 rng{42};
        std::shuffle(v.begin(), v.end(), rng);
        // 删掉偶数：连续内存一次 compact 往往快过链表逐节点 free
        std::erase_if(v, [](int x) { return x % 2 == 0; });
        assert(v.size() == 500);
        assert(std::ranges::all_of(v, [](int x) { return x % 2 != 0; }));
        std::cout << "erase_if on vector: simple + cache friendly\n";
    }

    std::cout << "[container_choice_cache_complexity] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01/container_choice_cache_complexity", run>;

}  // namespace

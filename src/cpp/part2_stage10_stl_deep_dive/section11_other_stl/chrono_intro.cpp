// Topic    : <chrono> 入门 — 时钟 / 时间点 / 时长
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : chrono_intro
// Topic id : part2/stage10/section11/chrono_intro
// Refs     : https://en.cppreference.com/w/cpp/chrono
//            https://en.cppreference.com/w/cpp/chrono/duration
//            https://en.cppreference.com/w/cpp/chrono/time_point

#include "learn/topic_registry.hpp"

#include <chrono>
#include <ctime>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    using namespace std::chrono_literals;
    std::cout << "=== [chrono_intro] duration / time_point / clocks ===\n";

    // ① 时长字面值与换算
    {
        const std::chrono::milliseconds ms = 1500ms;
        [[maybe_unused]] const auto sec = std::chrono::duration_cast<std::chrono::seconds>(ms);
        assert(sec.count() == 1);  // 截断向零
        assert((2000ms).count() == 2000);
        assert(1s == 1000ms);
        assert(2min == 120s);
        assert(1h == 60min);
        std::cout << "  literals: 1s=1000ms, 2min=120s, 1h=60min\n";
    }

    // ② 算术: duration 可加减、与标量乘除
    {
        auto d = 100ms + 50ms;
        assert(d == 150ms);
        d *= 2;
        assert(d == 300ms);
        std::cout << "  arithmetic: 100ms+50ms *2 = " << d.count() << "ms\n";
    }

    // ③ steady_clock: 单调时钟, 适合测耗时(不受系统校时影响)
    {
        const auto t0 = std::chrono::steady_clock::now();
        // 做一点确定工作(避免依赖真实 sleep 的脆弱性)
        volatile int sink = 0;
        for (int i = 0; i < 1000; ++i) {
            sink += i;
        }
        const auto t1 = std::chrono::steady_clock::now();
        const auto elapsed = t1 - t0;
        assert(elapsed >= std::chrono::nanoseconds{0});
        const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
        std::cout << "  steady_clock loop elapsed_ns=" << ns << " sink=" << sink << '\n';
    }

    // ④ system_clock: 可转 time_t 的墙钟
    {
        const auto now = std::chrono::system_clock::now();
        const std::time_t tt = std::chrono::system_clock::to_time_t(now);
        assert(tt > 0);
        std::cout << "  system_clock time_t=" << tt << '\n';
    }

    // ⑤ duration_cast vs floor/ceil/round(C++17)
    {
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        using std::chrono::seconds;
        [[maybe_unused]] const milliseconds x{1500};
        assert(duration_cast<seconds>(x).count() == 1);
        assert(std::chrono::floor<seconds>(x).count() == 1);
        assert(std::chrono::ceil<seconds>(x).count() == 2);
        assert(std::chrono::round<seconds>(x).count() == 2);
        std::cout << "  cast/floor/ceil/round on 1500ms\n";
    }

    std::cout << "[chrono_intro] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/chrono_intro", run>;

}  // namespace

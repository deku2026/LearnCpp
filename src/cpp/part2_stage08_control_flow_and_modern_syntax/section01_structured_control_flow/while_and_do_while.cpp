// Topic    : while / do-while 循环
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 1
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : while_and_do_while
// Topic id : part2/stage08/section01/while_and_do_while
// Refs     : https://en.cppreference.com/w/cpp/language/while
//            https://en.cppreference.com/w/cpp/language/do
//            ISO [stmt.while] [stmt.do]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

// 模拟“至少读一次输入”的 do-while 场景。
int read_positive_once(const std::vector<int>& feed, std::size_t& idx) {
    int value = 0;
    do {
        assert(idx < feed.size());
        value = feed[idx++];
        // 业务：跳过非正数，直到读到正数或 feed 用尽（此处保证有正数）。
    } while (value <= 0 && idx < feed.size());
    return value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [while_and_do_while] 入门：先判断 vs 后判断 ===\n";
    {
        // while：条件在循环体之前求值；一开始就假则体一次都不执行。
        int n = 3;
        int sum = 0;
        while (n > 0) {
            sum += n;
            --n;
        }
        assert(sum == 6 && n == 0);
        std::cout << "while sum 3+2+1=" << sum << '\n';

        // do-while：体至少执行一次，条件在体之后。
        int m = 0;
        int rounds = 0;
        do {
            ++rounds;
            ++m;
        } while (m < 1);  // 进循环时 m==0，体执行后 m==1，条件假 → 只一轮
        assert(rounds == 1 && m == 1);
        std::cout << "do-while always runs at least once, rounds=" << rounds << '\n';

        // 对比：若一开始条件就假，while 体不跑；do-while 仍跑一次。
        int while_hits = 0;
        int t = 0;
        while (t > 0) {
            ++while_hits;
            --t;
        }
        assert(while_hits == 0);

        int do_hits = 0;
        t = 0;
        do {
            ++do_hits;
        } while (t > 0);
        assert(do_hits == 1);
        std::cout << "zero-trip: while=" << while_hits << " do-while=" << do_hits << '\n';
    }

    std::cout << "=== 进阶：break / continue 与“条件里声明” ===\n";
    {
        // continue：跳到下次条件判断；break：离开循环。
        int i = 0;
        int seen_even = 0;
        while (i < 10) {
            ++i;
            if (i % 2 != 0) {
                continue;  // 奇数跳过
            }
            ++seen_even;
            if (i >= 8) {
                break;  // 到 8 就停
            }
        }
        // 偶数：2,4,6,8 → 4 次；在 8 时 break
        assert(seen_even == 4);
        std::cout << "even count with continue/break=" << seen_even << '\n';

        // while 条件也可以是声明（C++98）：变量作用域是循环体 + 条件本身。
        std::string s = "abc";
        std::size_t pos = 0;
        int chars = 0;
        while (char c = (pos < s.size() ? s[pos++] : '\0')) {
            // c 为 '\0' 时条件为假，循环结束；不会把空字符计入
            (void)c;
            ++chars;
        }
        assert(chars == 3);
        std::cout << "while (char c = ...) counted " << chars << " chars\n";

        // 典型 do-while：菜单/重试/至少处理一帧。
        std::vector<int> feed{-1, 0, 7, 9};
        std::size_t idx = 0;
        const int first_pos = read_positive_once(feed, idx);
        assert(first_pos == 7);
        std::cout << "do-while skipped non-positives, got " << first_pos << '\n';
    }

    std::cout << "=== 专家：死循环形态、与 for 的语义对照、空循环体 ===\n";
    {
        // 常见死循环写法：while (true) / for (;;) —— 需内部 break/return/throw 退出。
        int guard = 0;
        while (true) {
            ++guard;
            if (guard == 5) {
                break;
            }
        }
        assert(guard == 5);

        // 空循环体：条件副作用完成全部工作（可读性差，面试常见，生产慎用）。
        int x = 5;
        while (--x) {
            // 空体：x 从 4 递减到 0 时条件假
        }
        assert(x == 0);

        // while 与经典 for 对照：
        // for (init; cond; iter) stmt ≈ { init; while (cond) { stmt; iter; } }
        // 但 continue 时 for 仍会执行 iter，while 则直接回条件——这是语义差异。
        [[maybe_unused]] int for_like = 0;
        for (int k = 0; k < 3; ++k) {
            if (k == 1) {
                continue;  // 仍会 ++k
            }
            for_like += k;
        }
        // k=0 → +0; k=1 continue; k=2 → +2 → 2
        assert(for_like == 2);

        [[maybe_unused]] int while_like = 0;
        int k = 0;
        while (k < 3) {
            if (k == 1) {
                ++k;  // 必须手动推进，否则死循环
                continue;
            }
            while_like += k;
            ++k;
        }
        assert(while_like == 2);
        std::cout << "continue differs: for still runs iter; while does not\n";

        // 工程：优先 for 表达“有界计数/迭代器”；while 表达“事件/条件驱动”；
        // do-while 表达“至少一次 + 再决定是否重复”。
    }

    std::cout << "[while_and_do_while] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/while_and_do_while", run>;

}  // namespace

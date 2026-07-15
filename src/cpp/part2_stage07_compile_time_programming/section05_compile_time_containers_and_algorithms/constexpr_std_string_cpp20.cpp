// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_std_string_cpp20
// Topic id : part2/stage07/section05/constexpr_std_string_cpp20
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string
//   P0980R1 — Making std::string constexpr
//   同样受 transient allocation 约束

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <string_view>

namespace {

// ---------------------------------------------------------------------------
// §入门 — constexpr std::string 基本操作
// ---------------------------------------------------------------------------

constexpr std::size_t concat_len() {
    std::string s = "compile";
    s += '-';
    s += "time";
    return s.size();  // 12
}

constexpr char middle_char() {
    std::string s = "ABCDEFG";
    return s[s.size() / 2];
}

// ---------------------------------------------------------------------------
// §进阶 — 解析 / 校验 / 变换（编译期）
// ---------------------------------------------------------------------------

constexpr bool is_valid_version(std::string_view sv) {
    // 形如 d+.d+.d+
    int dots = 0;
    bool digit_run = false;
    for (char c : sv) {
        if (c == '.') {
            if (!digit_run) {
                return false;
            }
            ++dots;
            digit_run = false;
        } else if (c >= '0' && c <= '9') {
            digit_run = true;
        } else {
            return false;
        }
    }
    return digit_run && dots == 2;
}

constexpr int count_char(std::string_view sv, char ch) {
    std::string s{sv};  // 拷进 string 做算法
    return static_cast<int>(std::count(s.begin(), s.end(), ch));
}

constexpr auto upper_ascii_to_array() {
    std::string s = "AbC";
    for (char& c : s) {
        if (c >= 'a' && c <= 'z') {
            c = static_cast<char>(c - 'a' + 'A');
        }
    }
    std::array<char, 3> out{};
    for (std::size_t i = 0; i < 3; ++i) {
        out[i] = s[i];
    }
    return out;
}

// ---------------------------------------------------------------------------
// §专家 — SSO vs 堆；string_view；不能持久化 constexpr string
// ---------------------------------------------------------------------------
// 短字符串可能 SSO（无堆），长字符串触发 transient 堆分配。
// 无论 SSO 与否，作为 *命名空间作用域 constexpr 变量* 持有 string
// 在模型上仍受「分配不能逃逸」约束（实现会拒绝非瞬态分配）。
// 把结果带出常量求值：size/char/array/整型，而不是 string 本身。

// constexpr std::string kNone{"persist me"}; // 不要指望可移植地合法

constexpr int hash_djb2(std::string_view sv) {
    // 经典 djb2，便于 static_assert
    unsigned hash = 5381;
    for (unsigned char c : sv) {
        hash = ((hash << 5) + hash) + c;
    }
    return static_cast<int>(hash & 0x7fffffff);
}

constexpr int build_and_hash() {
    std::string s;
    s.reserve(16);
    s.append("cpp");
    s.push_back('2');
    s.push_back('3');
    return hash_djb2(s);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section05/constexpr_std_string_cpp20 ===\n";

    static_assert(concat_len() == 12);
    static_assert(middle_char() == 'D');
    static_assert(is_valid_version("1.2.3"));
    static_assert(!is_valid_version("1.2"));
    static_assert(!is_valid_version("10.b.3"));
    static_assert(count_char("banana", 'a') == 3);
    constexpr auto up = upper_ascii_to_array();
    static_assert(up[0] == 'A' && up[1] == 'B' && up[2] == 'C');
    static_assert(build_and_hash() == hash_djb2("cpp23"));

    std::cout << "[intro] concat_len=" << concat_len() << " middle_char=" << middle_char() << '\n';
    std::cout << "[advanced] version 1.2.3 valid? " << is_valid_version("1.2.3")
              << " count 'a' in banana=" << count_char("banana", 'a') << '\n';
    std::cout << "[expert] upper(AbC)=" << up[0] << up[1] << up[2] << " hash(cpp23)=" << build_and_hash()
              << "; return non-allocating results only\n";

    std::string rt = "compile";
    rt += "-time";
    assert(rt.size() == 12);

    std::cout << "constexpr_std_string_cpp20: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_std_string_cpp20", run>;

}  // namespace

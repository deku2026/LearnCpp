// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.6.2 sanitizers 通道 asan-ubsan
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : asan_ubsan_lane
// Topic id : part4/section06/asan_ubsan_lane
//
// 要点: 一条 CI lane: -fsanitize=address,undefined; 对照 Sanitizers.cmake。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct LaneFlags {
    std::vector<std::string> sanitize;
    bool omit_frame_pointer;
    bool debug_info;
};

LaneFlags asan_ubsan_linux() {
    return {{"address", "undefined"}, true, true};
}

std::string join_fsanitize(const LaneFlags& f) {
    std::string s = "-fsanitize=";
    for (std::size_t i = 0; i < f.sanitize.size(); ++i) {
        if (i) {
            s += ',';
        }
        s += f.sanitize[i];
    }
    return s;
}

// 仓库选项
bool repo_options_match(bool enable_asan, bool enable_ubsan) {
    // LEARNCPP_ENABLE_ASAN / LEARNCPP_ENABLE_UBSAN
    return enable_asan || enable_ubsan;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== asan_ubsan_lane ===\n";

    auto lane = asan_ubsan_linux();
    auto flags = join_fsanitize(lane);
    assert(flags == "-fsanitize=address,undefined");
    assert(lane.omit_frame_pointer);
    assert(lane.debug_info);
    std::cout << "  " << flags << " -fno-omit-frame-pointer -g\n";

    // linux-ci: LEARNCPP_ENABLE_UBSAN=ON
    assert(repo_options_match(false, true));
    std::cout << "  preset linux-ci enables UBSAN; combine ASan in sanitizer job\n";

    // 与 TSan 分道
    assert(flags.find("thread") == std::string::npos);

    // 安全烟雾: 有界访问(同 asan_overview)
    int a[3] = {1, 2, 3};
    int sum = 0;
    for (int i = 0; i < 3; ++i) {
        sum += a[i];
    }
    assert(sum == 6);

    std::cout << "asan_ubsan_lane: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/asan_ubsan_lane", run>;

}  // namespace

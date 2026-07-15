// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.2 Sanitizer 混用纪律
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : sanitizer_compatibility_matrix
// Topic id : part4/section05/sanitizer_compatibility_matrix
//
// 要点: ASan+UBSan 可同开; TSan 单独; MSan 高门槛单独; 不进 Release。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

enum class San { Asan, Ubsan, Tsan, Msan, Lsan };

bool compatible(San a, San b) {
    if (a == b) {
        return true;
    }
    // 规范化顺序
    if (static_cast<int>(a) > static_cast<int>(b)) {
        return compatible(b, a);
    }
    // ASan + UBSan OK
    if (a == San::Asan && b == San::Ubsan) {
        return true;
    }
    // ASan + LSan OK (LSan 常附带)
    if (a == San::Asan && b == San::Lsan) {
        return true;
    }
    // TSan 与 ASan/MSan 互斥
    if (a == San::Tsan || b == San::Tsan) {
        if (a == San::Tsan && b == San::Ubsan) {
            // 有时文档写可试, 但实践常单独; 本课按「TSan 单独通道」
            return false;
        }
        return false;
    }
    // MSan 与几乎所有其它互斥
    if (a == San::Msan || b == San::Msan) {
        return false;
    }
    return false;
}

struct Lane {
    std::string name;
    std::vector<San> sans;
    bool for_release;
};

bool lane_valid(const Lane& lane) {
    if (lane.for_release) {
        return false;  // 纪律: sanitizer 不进 Release
    }
    for (std::size_t i = 0; i < lane.sans.size(); ++i) {
        for (std::size_t j = i + 1; j < lane.sans.size(); ++j) {
            if (!compatible(lane.sans[i], lane.sans[j])) {
                return false;
            }
        }
    }
    return true;
}

const char* name(San s) {
    switch (s) {
        case San::Asan:
            return "ASan";
        case San::Ubsan:
            return "UBSan";
        case San::Tsan:
            return "TSan";
        case San::Msan:
            return "MSan";
        case San::Lsan:
            return "LSan";
    }
    return "?";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== sanitizer_compatibility_matrix ===\n";

    assert(compatible(San::Asan, San::Ubsan));
    assert(compatible(San::Asan, San::Lsan));
    assert(!compatible(San::Asan, San::Tsan));
    assert(!compatible(San::Asan, San::Msan));
    assert(!compatible(San::Tsan, San::Msan));

    Lane good{"asan-ubsan", {San::Asan, San::Ubsan}, false};
    Lane tsan{"tsan", {San::Tsan}, false};
    Lane bad_mix{"asan+tsan", {San::Asan, San::Tsan}, false};
    Lane bad_rel{"asan-release", {San::Asan}, true};

    assert(lane_valid(good));
    assert(lane_valid(tsan));
    assert(!lane_valid(bad_mix));
    assert(!lane_valid(bad_rel));

    std::cout << "  OK lanes: " << good.name << ", " << tsan.name << '\n';
    std::cout << "  reject: ASan+TSan, any sanitizer in Release\n";

    // 平台: Windows 本机 ASan; Linux CI ASan+UBSan + 独立 TSan
    std::cout << "  LearnCpp map: MSVC/clang-cl ASan; linux-ci UBSAN; TSan separate\n";

    // 打印矩阵一角
    const San all[] = {San::Asan, San::Ubsan, San::Tsan, San::Msan, San::Lsan};
    for (std::size_t i = 0; i < 5; ++i) {
        for (std::size_t j = i + 1; j < 5; ++j) {
            std::cout << "  " << name(all[i]) << "+" << name(all[j]) << "=" << (compatible(all[i], all[j]) ? "Y" : "n")
                      << '\n';
        }
    }

    std::cout << "sanitizer_compatibility_matrix: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/sanitizer_compatibility_matrix", run>;

}  // namespace

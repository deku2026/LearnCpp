// Topic    : strict aliasing 违规 vs std::bit_cast 修复（验收点）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.3
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : strict_aliasing_violation
// Topic id : part2/stage09/section08/strict_aliasing_violation
// Refs     : https://en.cppreference.com/w/cpp/numeric/bit_cast
//            https://en.cppreference.com/w/cpp/language/reinterpret_cast
//            ISO [basic.lval] [bit.cast]
//            Shafik Yaghmour: Strict Aliasing Rule

#include "learn/topic_registry.hpp"

#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

namespace {

// ✅ 正确：bit_cast 安全类型双关
float bits_to_float(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}

std::uint32_t float_to_bits(float f) {
    return std::bit_cast<std::uint32_t>(f);
}

// ✅ 正确：memcpy 也是标准认可的 type punning 手段
[[maybe_unused]] float bits_to_float_memcpy(std::uint32_t bits) {
    float f;
    std::memcpy(&f, &bits, sizeof(f));
    return f;
}

// 经典 fast inverse sqrt 的位级部分（用 bit_cast，无 UB）
float fast_inv_sqrt_step(float x) {
    const std::uint32_t i = std::bit_cast<std::uint32_t>(x);
    const std::uint32_t ybits = 0x5f3759dfu - (i >> 1);
    float y = std::bit_cast<float>(ybits);
    y = y * (1.5f - 0.5f * x * y * y);
    return y;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [strict_aliasing_violation] 主干：bit_cast 往返 ===\n";
    {
        const float one = 1.0f;
        const std::uint32_t bits = float_to_bits(one);
        // IEEE754 1.0f 位模式 0x3f800000
        assert(bits == 0x3f800000u);
        assert(bits_to_float(bits) == 1.0f);
        assert(bits_to_float_memcpy(bits) == 1.0f);
        std::cout << "1.0f bits=0x" << std::hex << bits << std::dec << '\n';
    }

    std::cout << "=== 对抗：序列化 float 组（无 UB）===\n";
    {
        const std::vector<float> src{1.0f, 2.5f, -0.0f};
        std::vector<std::uint32_t> raw;
        raw.reserve(src.size());
        for (float f : src) {
            raw.push_back(float_to_bits(f));
        }
        std::vector<float> dst;
        for (auto b : raw) {
            dst.push_back(bits_to_float(b));
        }
        assert(dst.size() == src.size());
        assert(dst[0] == 1.0f && dst[1] == 2.5f);
        std::cout << "round-trip " << dst.size() << " floats via bit_cast\n";
    }

    std::cout << "=== 对抗：bit_cast 版 fast inv sqrt 一步 ===\n";
    {
        const float y = fast_inv_sqrt_step(4.0f);
        // 约 0.5，允许近似
        assert(y > 0.4f && y < 0.6f);
        std::cout << "fast_inv_sqrt_step(4)≈" << y << '\n';
    }

    std::cout << "=== 专节：违规形态（不执行）与工具局限 ===\n";
    // 危险（勿运行 / 视为 UB）：
    //   float bad(int bits) {
    //     return *reinterpret_cast<float*>(&bits); // strict aliasing 违规
    //   }
    // union 读非活跃成员在 C++ 也是 UB（C 合法）。
    // char/unsigned char/std::byte 可别名查看对象表示；反过来当 float 读仍要用
    // bit_cast/memcpy。
    // 诚实：简单场景 -O0/-O2 可能“碰巧相同”；UBSan 也不保证抓 aliasing。
    // 纪律：类型双关一律 bit_cast/memcpy。
    std::cout << "never type-pun via reinterpret_cast pointers; use bit_cast\n";
    std::cout << "godbolt: compare bad punning codegen at -O0 vs -O2\n";

    std::cout << "[strict_aliasing_violation] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/strict_aliasing_violation", run>;

}  // namespace

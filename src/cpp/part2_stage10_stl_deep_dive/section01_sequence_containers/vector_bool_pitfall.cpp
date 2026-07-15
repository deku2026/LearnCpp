// Topic    : vector<bool> 特化陷阱（proxy / 非真正容器）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 1.2
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : vector_bool_pitfall
// Topic id : part2/stage10/section01/vector_bool_pitfall
// Refs     : https://en.cppreference.com/w/cpp/container/vector_bool
//            https://en.cppreference.com/w/cpp/container/vector
//            ISO [vector.bool]

#include "learn/topic_registry.hpp"

#include <array>
#include <bitset>
#include <deque>
#include <iostream>
#include <type_traits>
#include <vector>

namespace {

// 期望拿到 bool& 的泛型写错场景
template <class Cont>
void flip_first_as_ref(Cont& c) {
    // 对 vector<bool>：c[0] 是 proxy，不能绑 bool&
    // 这里用 auto 接 proxy，再写回
    auto x = c[0];
    c[0] = !static_cast<bool>(x);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [vector_bool_pitfall] 入门：operator[] 返回 proxy 不是 bool& ===\n";
    {
        std::vector<bool> vb{true, false, true};
        assert(vb.size() == 3);
        assert(vb[0] == true);
        assert(vb[1] == false);

        // auto 推导为 vector<bool>::reference（代理），不是 bool
        auto bit = vb[1];
        bit = true;  // 通过代理写回底层位
        assert(vb[1] == true);

        // 与真正 bool 容器对照
        std::vector<char> flags{1, 0, 1};
        char& r = flags[1];  // 真引用 OK
        r = 1;
        assert(flags[1] == 1);
        std::cout << "vector<bool>[i] is proxy; vector<char>[i] is real ref\n";
    }

    std::cout << "=== 进阶：不能取 bool* / 不能绑 bool& ===\n";
    {
        std::vector<bool> vb{false, true};
        // bool& ref = vb[0];          // ❌ 编译失败：proxy 不能绑 bool&
        // bool* p   = &vb[0];         // ❌ 没有真正的 bool 对象可取址

        using Ref = decltype(vb[0]);
        static_assert(!std::is_same_v<Ref, bool&>);
        static_assert(!std::is_same_v<Ref, bool>);
        std::cout << "proxy type is neither bool nor bool&\n";

        // 需要值语义时显式转换
        bool value = static_cast<bool>(vb[0]);
        assert(value == false);
        (void)value;
    }

    std::cout << "=== 进阶：与算法/模板的摩擦 ===\n";
    {
        std::vector<bool> vb{true, false, true, false};
        flip_first_as_ref(vb);
        assert(vb[0] == false);

        // data()：vector<bool> 没有 data()（C++ 标准不提供）
        // 因为没有连续 bool 对象数组
        std::vector<char> real{1, 0, 1};
        assert(real.data() != nullptr);
        std::cout << "vector<bool> has no data(); real containers do\n";
    }

    std::cout << "=== 专家：替代方案选型 ===\n";
    {
        // 1) 动态 + 真 bool/字节语义
        std::vector<char> as_bytes{0, 1, 0, 1};
        std::deque<bool> as_deque{false, true, false, true};  // 真容器语义
        assert(as_bytes[1] == 1);
        assert(as_deque[1] == true);

        // 2) 固定大小位图
        std::bitset<8> bits;
        bits.set(1);
        bits.set(3);
        assert(bits.test(1));
        assert(bits.count() == 2);

        // 3) 固定大小真 bool 数组
        std::array<bool, 4> arr{false, true, false, true};
        bool& ar = arr[1];
        ar = false;
        assert(arr[1] == false);

        std::cout << "prefer vector<char>/deque<bool>/bitset/array<bool>\n";
        std::cout << "use vector<bool> only when bit-packing is intentional\n";
    }

    std::cout << "=== 专家：何时仍可用 vector<bool> ===\n";
    {
        // 海量标志位且内存敏感、不需要取址/泛型写 bool& 时
        std::vector<bool> huge(1024, false);
        huge[100] = true;
        assert(huge[100]);
        // 空间约 1024 bit vs vector<char> 1024 字节
        std::cout << "vector<bool> packs bits; OK for dense flags only\n";
    }

    std::cout << "[vector_bool_pitfall] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01/vector_bool_pitfall", run>;

}  // namespace

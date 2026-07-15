// Topic    : std::start_lifetime_as（C++23 显式开始对象生命周期）
// Doc      : 第3部分-标准库系统化.md · 库 6.4
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : start_lifetime_as_cpp23
// Topic id : part3/section06/start_lifetime_as_cpp23
// Refs     : https://en.cppreference.com/w/cpp/memory/start_lifetime_as
//            P2590；__cpp_lib_start_lifetime_as

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <new>
#include <type_traits>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

struct Packet {
    std::uint32_t magic;
    std::uint32_t length;
};

static_assert(std::is_trivially_copyable_v<Packet>);

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [start_lifetime_as_cpp23] ===\n";

    // 模拟从网络/文件读到的原始字节
    alignas(Packet) unsigned char raw[sizeof(Packet)]{};
    {
        Packet src{0xA1B2C3D4u, 16u};
        std::memcpy(raw, &src, sizeof(src));
    }

#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L
    std::cout << "__cpp_lib_start_lifetime_as=" << __cpp_lib_start_lifetime_as << '\n';

    // 合法地让 raw 存储开始作为 Packet 对象的生命周期
    Packet* p = std::start_lifetime_as<Packet>(raw);
    assert(p != nullptr);
    assert(p->magic == 0xA1B2C3D4u);
    assert(p->length == 16u);
    std::cout << "start_lifetime_as Packet magic=" << std::hex << p->magic << std::dec << " length=" << p->length
              << '\n';

    // 数组版本（若实现提供）
#if defined(__cpp_lib_start_lifetime_as_array)
    alignas(int) unsigned char arr_raw[sizeof(int) * 4]{};
    int pattern[4] = {1, 2, 3, 4};
    std::memcpy(arr_raw, pattern, sizeof(pattern));
    int* arr = std::start_lifetime_as_array<int>(arr_raw, 4);
    assert(arr[0] == 1 && arr[3] == 4);
    std::cout << "start_lifetime_as_array sum=" << (arr[0] + arr[1] + arr[2] + arr[3]) << '\n';
#endif
#else
    std::cout << "start_lifetime_as not available; memcpy + local object fallback\n";
    // 可移植、已定义的做法：拷到真实对象（隐式生命周期类型）
    Packet p{};
    std::memcpy(&p, raw, sizeof(p));
    assert(p.magic == 0xA1B2C3D4u && p.length == 16u);
    std::cout << "memcpy into Packet length=" << p.length << '\n';
    std::cout << "reinterpret_cast on raw storage without lifetime is classic UB risk\n";
#endif

    std::cout << "use when: bytes from IO become typed objects (implicit-lifetime T)\n";
    std::cout << "[start_lifetime_as_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/start_lifetime_as_cpp23", run>;

}  // namespace

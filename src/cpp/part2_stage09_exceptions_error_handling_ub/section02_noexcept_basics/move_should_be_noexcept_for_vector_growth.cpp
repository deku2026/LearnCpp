// Topic    : 移动为何要 noexcept：vector 扩容与 move_if_noexcept
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 2.3（验收点）
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section02_noexcept_basics
// Item     : move_should_be_noexcept_for_vector_growth
// Topic id : part2/stage09/section02/move_should_be_noexcept_for_vector_growth
// Refs     : https://en.cppreference.com/w/cpp/utility/move_if_noexcept
//            https://en.cppreference.com/w/cpp/container/vector
//            ISO [vector.capacity] [except.spec]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

int g_copies = 0;
int g_moves = 0;

struct FastMove {
    int id = 0;
    FastMove() = default;
    explicit FastMove(int i) : id(i) {}
    FastMove(const FastMove& o) : id(o.id) { ++g_copies; }
    FastMove(FastMove&& o) noexcept : id(o.id) {
        ++g_moves;
        o.id = -1;
    }
    FastMove& operator=(const FastMove& o) {
        id = o.id;
        ++g_copies;
        return *this;
    }
    FastMove& operator=(FastMove&& o) noexcept {
        id = o.id;
        o.id = -1;
        ++g_moves;
        return *this;
    }
};

struct SlowMove {
    int id = 0;
    SlowMove() = default;
    explicit SlowMove(int i) : id(i) {}
    SlowMove(const SlowMove& o) : id(o.id) { ++g_copies; }
    // 故意不标 noexcept：vector 为保强保证，扩容时改用拷贝
    SlowMove(SlowMove&& o) : id(o.id) {
        ++g_moves;
        o.id = -1;
    }
    SlowMove& operator=(const SlowMove& o) {
        id = o.id;
        ++g_copies;
        return *this;
    }
    SlowMove& operator=(SlowMove&& o) {
        id = o.id;
        o.id = -1;
        ++g_moves;
        return *this;
    }
};

class Buffer {
    int* data_ = nullptr;
    std::size_t size_ = 0;

public:
    Buffer() = default;
    explicit Buffer(std::size_t n) : data_(new int[n]{}), size_(n) {}
    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& o) : data_(new int[o.size_]{}), size_(o.size_) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = o.data_[i];
        }
    }
    Buffer(Buffer&& o) noexcept : data_(o.data_), size_(o.size_) {
        o.data_ = nullptr;
        o.size_ = 0;
    }
    Buffer& operator=(Buffer o) noexcept {
        // copy-and-swap 简化版（详见 section03）
        std::swap(data_, o.data_);
        std::swap(size_, o.size_);
        return *this;
    }

    std::size_t size() const noexcept { return size_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [move_should_be_noexcept_for_vector_growth] 主干 ===\n";
    {
        static_assert(std::is_nothrow_move_constructible_v<FastMove>);
        static_assert(!std::is_nothrow_move_constructible_v<SlowMove>);
        static_assert(std::is_nothrow_move_constructible_v<Buffer>);

        g_copies = g_moves = 0;
        {
            std::vector<FastMove> v;
            v.reserve(1);
            v.emplace_back(1);
            const int moves_before = g_moves;
            v.emplace_back(2);  // 扩容：应移动旧元素
            assert(g_moves > moves_before);
            std::cout << "FastMove growth moves=" << g_moves << " copies=" << g_copies << '\n';
        }

        g_copies = g_moves = 0;
        {
            std::vector<SlowMove> v;
            v.reserve(1);
            v.emplace_back(1);
            const int copies_before = g_copies;
            v.emplace_back(2);  // 扩容：移动非 noexcept → 拷贝以保强保证
            assert(g_copies > copies_before);
            std::cout << "SlowMove growth moves=" << g_moves << " copies=" << g_copies << '\n';
        }
    }

    std::cout << "=== 对抗：std::move_if_noexcept ===\n";
    {
        FastMove a{10};
        auto b = std::move_if_noexcept(a);
        assert(b.id == 10);
        // nothrow → 走移动，源被掏空
        assert(a.id == -1);

        SlowMove c{20};
        auto d = std::move_if_noexcept(c);
        assert(d.id == 20);
        // 可抛移动 + 可拷贝 → 拷贝，源保持
        assert(c.id == 20);
        std::cout << "move_if_noexcept: Fast moved, Slow copied\n";
    }

    std::cout << "=== 专节：结论 ===\n";
    // vector 扩容时用 move_if_noexcept：只有 noexcept 移动才用移动（快），
    // 否则拷贝（慢）——因为移动半途抛异常无法回滚，会破坏强异常保证。
    // 你的类型移动构造/赋值只要真不抛，就应标 noexcept。
    std::cout << "mark move ctor/assign noexcept when they truly never throw\n";

    std::cout << "[move_should_be_noexcept_for_vector_growth] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage09/section02/move_should_be_noexcept_for_vector_growth", run>;

}  // namespace

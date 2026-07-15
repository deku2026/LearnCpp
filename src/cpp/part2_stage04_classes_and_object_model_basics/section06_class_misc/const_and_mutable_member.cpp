// Topic     : const 成员与 mutable
// Doc       : 第2部分-阶段4 · 步骤 8.2
// cppreference: https://en.cppreference.com/cpp/language/cv
//
// 要点: const 数据必须初始化后不可改；mutable 允许在 const 方法里改实现细节（缓存/计数/锁）。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <mutex>
#include <string>

namespace {

class ImmutableId {
    const int id_;

public:
    explicit ImmutableId(int id) : id_(id) {}
    int id() const { return id_; }
    // void set(int) { id_ = 1; } // ❌
};

class Cache {
    mutable std::mutex mtx_;
    mutable int hits_ = 0;
    mutable std::string cached_;
    std::string value_;

public:
    explicit Cache(std::string v) : value_(std::move(v)) {}

    // 逻辑 const：对外不改 value_，但可更新缓存统计
    const std::string& get() const {
        std::lock_guard<std::mutex> lk(mtx_);
        ++hits_;
        cached_ = value_;  // 示意缓存
        return value_;
    }

    int hits() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return hits_;
    }

    void set(std::string v) {
        std::lock_guard<std::mutex> lk(mtx_);
        value_ = std::move(v);
        cached_.clear();
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [const_and_mutable_member] ===\n";

    // -------------------------------------------------------------------------
    // §入门：const 数据成员
    // -------------------------------------------------------------------------
    ImmutableId id{42};
    assert(id.id() == 42);
    std::cout << "[intro] const id=" << id.id() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：const 对象调 const 方法，内部 mutable 可写
    // -------------------------------------------------------------------------
    const Cache c{"payload"};
    assert(c.get() == "payload");
    assert(c.get() == "payload");
    assert(c.hits() == 2);
    std::cout << "[advanced] const Cache hits=" << c.hits() << '\n';

    // -------------------------------------------------------------------------
    // §专家：bitwise const vs logical const
    // -------------------------------------------------------------------------
    // const 方法默认 bitwise const；mutable 是为 logical const 开的口子。
    // 勿用 mutable 偷改「对象抽象状态」——会破坏线程/推理。
    Cache w{"x"};
    w.set("y");
    assert(w.get() == "y");
    assert(w.hits() == 1);

    std::cout << "[expert] logical const via mutable; value=" << w.get() << '\n';
    std::cout << "=== const_and_mutable_member: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/const_and_mutable_member", run>;

}  // namespace

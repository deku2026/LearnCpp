// Topic     : RAII 与异常安全配对
// Doc       : 第2部分-阶段4 · 步骤 2.2
// cppreference: https://en.cppreference.com/cpp/language/raii
//
// 要点: 手动 lock/unlock 在异常与早退路径会泄漏；lock_guard/unique_lock 把释放绑到析构；
//       多资源按构造逆序析构，天然配对。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

std::mutex g_mtx;
int g_unlocked_cleanly = 0;

void maybe_throw(bool do_throw) {
    if (do_throw) {
        throw std::runtime_error("boom");
    }
}

// 反面教材：手动锁（仅演示逻辑；真正调用会在异常路径死锁/泄漏锁）
// 本 topic 不实际跑 bad 路径的锁泄漏，而用自定义 Guard 计数验证。

class LockCounter {
    static inline int depth_ = 0;
    bool locked_ = false;

public:
    void lock() {
        ++depth_;
        locked_ = true;
    }
    void unlock() {
        assert(locked_);
        locked_ = false;
        --depth_;
        ++g_unlocked_cleanly;
    }
    static int depth() { return depth_; }
    bool owns() const { return locked_; }
};

class Guard {
    LockCounter* m_ = nullptr;

public:
    explicit Guard(LockCounter& m) : m_(&m) { m_->lock(); }
    ~Guard() {
        if (m_ && m_->owns()) {
            m_->unlock();
        }
    }
    Guard(const Guard&) = delete;
    Guard& operator=(const Guard&) = delete;
};

int work_with_guard(LockCounter& m, bool do_throw, bool early_return) {
    Guard g{m};
    if (early_return) {
        return 1;  // 析构仍 unlock
    }
    maybe_throw(do_throw);
    return 0;
}

// 多资源：构造顺序 A→B，析构 B→A
struct Trace {
    std::string name;
    std::string* log;
    explicit Trace(std::string n, std::string* l) : name(std::move(n)), log(l) { *log += '+' + name; }
    ~Trace() { *log += '-' + name; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [raii_exception_safety_pairing] ===\n";

    // -------------------------------------------------------------------------
    // §入门：正常路径 + 早退都释放
    // -------------------------------------------------------------------------
    LockCounter m;
    g_unlocked_cleanly = 0;
    assert(work_with_guard(m, false, false) == 0);
    assert(work_with_guard(m, false, true) == 1);
    assert(LockCounter::depth() == 0);
    assert(g_unlocked_cleanly == 2);
    std::cout << "[intro] early-return still unlocked, cleanly=" << g_unlocked_cleanly << '\n';

    // -------------------------------------------------------------------------
    // §进阶：异常路径仍 unlock
    // -------------------------------------------------------------------------
    try {
        work_with_guard(m, true, false);
        assert(false && "should throw");
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "boom");
    }
    assert(LockCounter::depth() == 0);
    assert(g_unlocked_cleanly == 3);
    std::cout << "[advanced] exception path unlocked, depth=0\n";

    // -------------------------------------------------------------------------
    // §专家：std::lock_guard + 析构逆序
    // -------------------------------------------------------------------------
    {
        std::lock_guard<std::mutex> lk(g_mtx);
        // 临界区：标准 lock_guard 与手写 Guard 同构——离开作用域自动 unlock
        assert(LockCounter::depth() == 0);  // 自定义计数器未参与此路径
        (void)lk;
    }  // unlock

    std::string log;
    {
        Trace a{"A", &log};
        Trace b{"B", &log};
        assert(log == "+A+B");
    }
    assert(log == "+A+B-B-A");  // 后构造先析构
    std::cout << "[expert] destruction order log=" << log << '\n';

    std::cout << "=== raii_exception_safety_pairing: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section02/raii_exception_safety_pairing", run>;

}  // namespace

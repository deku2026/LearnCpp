// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E5 Policy)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section05_crtp_and_policy_based_design
// Item     : policy_based_design
// Topic id : part6/e/section05/policy_based_design
//
// 要点: Policy-based design = 策略作为模板参数组合行为 (Alexandrescu);
//       编译期装配, 零运行时开销。
// 参考: Modern C++ Design; C++ Software Design

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>

namespace {

// 线程策略
struct SingleThreaded {
    struct Lock {
        explicit Lock(const SingleThreaded&) {}
    };
};

struct MultiThreaded {
    mutable std::mutex mtx;
    struct Lock {
        std::mutex& m;
        explicit Lock(const MultiThreaded& host) : m(host.mtx) { m.lock(); }
        ~Lock() { m.unlock(); }
        Lock(const Lock&) = delete;
        Lock& operator=(const Lock&) = delete;
    };
};

// 检查策略
struct NoChecking {
    static void check(const void*) {}
};

struct EnforceNotNull {
    static void check(const void* p) {
        if (!p) {
            throw std::runtime_error("null");
        }
    }
};

// 存储策略
template <typename T>
struct DefaultStorage {
    T* ptr = nullptr;
    T* get_raw() const { return ptr; }
    void set_raw(T* p) { ptr = p; }
};

// 装配
template <typename T, typename ThreadPolicy = SingleThreaded, typename CheckPolicy = NoChecking,
          template <typename> class StoragePolicy = DefaultStorage>
class SmartPtr : private ThreadPolicy, public StoragePolicy<T> {
public:
    explicit SmartPtr(T* p = nullptr) { this->set_raw(p); }
    ~SmartPtr() { delete this->get_raw(); }

    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;

    T& operator*() const {
        typename ThreadPolicy::Lock guard(*this);
        CheckPolicy::check(this->get_raw());
        return *this->get_raw();
    }

    T* get() const {
        typename ThreadPolicy::Lock guard(*this);
        return this->get_raw();
    }
};

// 另一组: 日志 policy
struct SilentLog {
    static void info(const std::string&) {}
};

struct CoutLog {
    static void info(const std::string& s) { std::cout << "  [log] " << s << '\n'; }
};

template <typename LogPolicy>
struct Service {
    void run() { LogPolicy::info("Service::run"); }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E5 policy-based design ===\n";

    SmartPtr<int> p(new int{42});
    assert(*p == 42);

    SmartPtr<int, SingleThreaded, EnforceNotNull> q(new int{7});
    assert(*q == 7);

    try {
        SmartPtr<int, SingleThreaded, EnforceNotNull> bad(nullptr);
        (void)*bad;
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "  EnforceNotNull caught null\n";
    }

    Service<SilentLog>{}.run();
    Service<CoutLog>{}.run();

    // 组合爆炸: policies 正交组合, 无需手写所有子类
    std::cout << "  policies compose at compile time; prefer concepts for constraints\n";
    std::cout << "policy_based_design: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section05/policy_based_design", run>;

}  // namespace

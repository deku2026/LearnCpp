// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §3.3
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : policy_based_design_idiom
// Topic id : part2/stage15/section03/policy_based_design_idiom
//
// Policy-based design: orthogonal behaviors as template policy parameters.
// Motive : compose check/thread/storage strategies at compile time (zero overhead).
// Modern : still used in high-perf libs; Allocator/Hash/Compare in STL are policies.
// Pitfall: policy parameter explosion; empty policies need EBO/no_unique_address.

#include "learn/topic_registry.hpp"

#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

// ---------- threading policies ----------
struct SingleThreaded {
    void lock() const noexcept {}
    void unlock() const noexcept {}
};

struct MultiThreaded {
    mutable std::mutex m;
    void lock() const { m.lock(); }
    void unlock() const { m.unlock(); }
};

// ---------- checking policies ----------
struct NoCheck {
    static void check_not_empty(const std::string&) {}
};

struct ThrowOnEmpty {
    static void check_not_empty(const std::string& s) {
        if (s.empty()) {
            throw std::invalid_argument("empty");
        }
    }
};

// ---------- host class parameterized by policies ----------
template <typename Threading = SingleThreaded, typename Checking = NoCheck>
class NameService : private Threading {
    std::string name_;

public:
    explicit NameService(std::string name) : name_(std::move(name)) {}

    void set_name(std::string name) {
        Checking::check_not_empty(name);
        this->lock();
        name_ = std::move(name);
        this->unlock();
    }

    std::string get_name() const {
        this->lock();
        std::string out = name_;
        this->unlock();
        return out;
    }
};

// RAII helper so unlock is exception-safe when combining policies for real.
template <typename Threading>
struct PolicyLock {
    const Threading& t;
    explicit PolicyLock(const Threading& th) : t(th) { t.lock(); }
    ~PolicyLock() { t.unlock(); }
};

template <typename Threading = SingleThreaded, typename Checking = NoCheck>
class SaferNameService : private Threading {
    std::string name_;

public:
    explicit SaferNameService(std::string name) : name_(std::move(name)) {}

    void set_name(std::string name) {
        Checking::check_not_empty(name);
        PolicyLock<Threading> guard(*this);
        name_ = std::move(name);
    }

    std::string get_name() const {
        PolicyLock<Threading> guard(*this);
        return name_;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== SingleThreaded + NoCheck ==\n";
    NameService<> a("alpha");
    a.set_name("beta");
    assert(a.get_name() == "beta");
    std::cout << "  " << a.get_name() << '\n';

    std::cout << "== MultiThreaded + ThrowOnEmpty ==\n";
    SaferNameService<MultiThreaded, ThrowOnEmpty> b("ok");
    b.set_name("gamma");
    assert(b.get_name() == "gamma");
    try {
        b.set_name("");
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << "  caught: " << e.what() << '\n';
    }

    std::cout << "compose orthogonal axes; avoid 5+ policies unless library-scale\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/policy_based_design_idiom", run>;

}  // namespace

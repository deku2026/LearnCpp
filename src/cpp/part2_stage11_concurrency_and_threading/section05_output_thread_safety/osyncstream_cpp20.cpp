// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section05_output_thread_safety
// Item     : osyncstream_cpp20
// Topic id : part2/stage11/section05/osyncstream_cpp20
//
// Covers: std::osyncstream (C++20)

#include "learn/topic_registry.hpp"

#include <mutex>
#include <sstream>
#include <string>
#include <syncstream>
#include <thread>

namespace {

void demo_basics() {
#if defined(__cpp_lib_syncbuf) && __cpp_lib_syncbuf >= 201803L
    std::ostringstream oss;
    {
        std::osyncstream synced(oss);
        synced << "hello";
    }
    LEARN_CHECK(oss.str() == "hello");
#else
    std::ostringstream oss;
    oss << "hello";
    LEARN_CHECK(oss.str() == "hello");
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_syncbuf) && __cpp_lib_syncbuf >= 201803L
    std::ostringstream oss;
    std::thread t1([&] {
        std::osyncstream s(oss);
        s << "A";
    });
    std::thread t2([&] {
        std::osyncstream s(oss);
        s << "B";
    });
    t1.join();
    t2.join();
    const std::string out = oss.str();
    LEARN_CHECK(out.size() == 2);
    LEARN_CHECK((out == "AB") || (out == "BA"));
#else
    std::mutex m;
    std::ostringstream oss;
    std::thread t1([&] {
        std::lock_guard<std::mutex> g(m);
        oss << "A";
    });
    std::thread t2([&] {
        std::lock_guard<std::mutex> g(m);
        oss << "B";
    });
    t1.join();
    t2.join();
    LEARN_CHECK(oss.str().size() == 2);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_syncbuf) && __cpp_lib_syncbuf >= 201803L
    std::ostringstream oss;
    {
        std::osyncstream s(oss);
        s << "x" << 1 << "y";
        s.emit();
        LEARN_CHECK(oss.str() == "x1y");
    }
#else
    std::ostringstream oss;
    oss << "x" << 1 << "y";
    LEARN_CHECK(oss.str() == "x1y");
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section05/osyncstream_cpp20", run>;

}  // namespace

// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 10.1
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section05_output_thread_safety
// Item     : osyncstream_cpp20
// Topic id : part2/stage11/section05/osyncstream_cpp20
// Refs     : https://en.cppreference.com/w/cpp/io/basic_osyncstream
//            P0053 · ISO [syncstream]
//
// osyncstream (C++20): buffer a transfer; flush atomically to the wrapped
// stream on destruction (or emit()). Prevents character-level interleaving.

#include "learn/topic_registry.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <syncstream>
#include <thread>
#include <vector>

namespace {

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [osyncstream] concurrent lines stay intact ===\n";
    {
        // Also write to cout so the learner sees ordered lines.
        {
            std::vector<std::jthread> pool;
            for (int id = 0; id < 6; ++id) {
                pool.emplace_back([id] { std::osyncstream(std::cout) << "thread " << id << " complete line\n"; });
            }
        }
        // Order among different osyncstream instances is unspecified,
        // but each transfer is not torn.
        std::cout << "  (each line above should be whole, not mixed mid-line)\n";
    }

    std::cout << "=== contrast: bare cout can interleave (not asserted) ===\n";
    std::cout << "  std::cout << \"thread \" << id << \" ...\\n\" is multiple\n"
                 "  insertions; concurrent threads may mix characters.\n";

    std::cout << "=== emit() flushes early without destroying ===\n";
    {
        std::osyncstream oss(std::cout);
        oss << "partial ";
        oss.emit();  // atomic transfer of current buffer
        oss << "then more\n";
        // dtor emits remainder
    }

    std::cout << "=== capture into stringbuf via osyncstream ===\n";
    {
        std::stringbuf buf;
        std::ostream shared(&buf);
        {
            std::jthread t1([&] { std::osyncstream(shared) << "AAAA\n"; });
            std::jthread t2([&] { std::osyncstream(shared) << "BBBB\n"; });
        }
        const std::string out = buf.str();
        // Either order, but both blocks present as whole units.
        assert(out.find("AAAA\n") != std::string::npos);
        assert(out.find("BBBB\n") != std::string::npos);
        assert(out.size() == 10);
        std::cout << "  stringbuf got two intact 5-char transfers\n";
    }

    std::cout << "[osyncstream_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section05/osyncstream_cpp20", run>;

}  // namespace

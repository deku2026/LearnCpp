// Topic    : format thread::id / stacktrace(P2693) + 基础可观测性
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 15.2③
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_thread_id_stacktrace_cpp23
// Topic id : part2/stage10/section10/std_format_thread_id_stacktrace_cpp23
// Refs     : https://en.cppreference.com/w/cpp/thread/thread/id
//            https://en.cppreference.com/w/cpp/utility/basic_stacktrace
//            P2693R1

#include "learn/topic_registry.hpp"

#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
#include <stacktrace>
#define LEARN_HAS_STACKTRACE 1
#else
#define LEARN_HAS_STACKTRACE 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_format_thread_id_stacktrace_cpp23] ===\n";

    // ① thread::id 可格式化(C++23 / P2693 明确支持; 多数实现更早可用 ostream)
    {
        const std::thread::id tid = std::this_thread::get_id();
        const std::string via_format = std::format("{}", tid);

        std::ostringstream oss;
        oss << tid;
        const std::string via_stream = oss.str();

        // format 与 ostream 表示应一致(实现定义字符串, 但两边应对齐)
        assert(!via_format.empty());
        assert(via_format == via_stream);
        std::cout << "  thread::id: " << via_format << '\n';
    }

    // ② stacktrace: 当前调用栈快照
#if LEARN_HAS_STACKTRACE
    {
        std::cout << "  __cpp_lib_stacktrace=" << __cpp_lib_stacktrace << '\n';
        const auto st = std::stacktrace::current();
        assert(!st.empty());

        // to_string / format 均可
        const std::string text = std::to_string(st);
        assert(!text.empty());
        std::cout << "  stacktrace frames=" << st.size() << '\n';

        // format stacktrace entry / whole stacktrace (若实现提供)
        try {
            const std::string fmt_all = std::format("{}", st);
            assert(!fmt_all.empty());
            std::cout << "  format(stacktrace) ok, chars=" << fmt_all.size() << '\n';
        } catch (const std::exception& ex) {
            std::cout << "  format(stacktrace) not available: " << ex.what() << '\n';
        }

        // 至少有一条 description
        const auto& top = st[0];
        const std::string desc = top.description();
        std::cout << "  top description nonempty? " << (!desc.empty() ? "yes" : "maybe-empty") << '\n';
    }
#else
    std::cout << "  (no <stacktrace>) thread::id only — stacktrace skipped\n";
#endif

    std::cout << "[std_format_thread_id_stacktrace_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section10/std_format_thread_id_stacktrace_cpp23", run>;

}  // namespace

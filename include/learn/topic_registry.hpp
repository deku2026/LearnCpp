// LearnCpp topic registry.
//
// Every placeholder `.cpp` instantiates `learn::topic<"path/like/id", run>`,
// which is an inline variable template whose constructor inserts (id, run)
// into a global `std::map` at static-init time. `main()` then dispatches.
#pragma once

// Topic demos use assert() as the primary verification path.
//
// CI builds RelWithDebInfo with -DNDEBUG. That would make <cassert>'s assert a
// no-op, then -Werror -Wunused-* fails on demo locals only referenced inside
// assert(...). Undefining NDEBUG globally also breaks main()'s Release path
// (no-arg should *list* topics, not run every topic).
//
// Solution: keep NDEBUG for main(), but redefine assert() to a learn:: check
// that is always active in every TU that includes this registry header.
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string_view>

namespace learn {
namespace detail {
[[noreturn]] inline void assert_fail(const char* expr, const char* file, int line) {
    std::fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", expr, file, line);
    std::abort();
}
}  // namespace detail
}  // namespace learn

// Override assert for topic TUs (and any TU including this header after <cassert>).
#undef assert
#define assert(expr) ((expr) ? (void)0 : ::learn::detail::assert_fail(#expr, __FILE__, __LINE__))

namespace learn {

using TopicFn = int (*)(int argc, char** argv);

namespace detail {
void register_topic_impl(std::string_view id, TopicFn fn) noexcept;
}  // namespace detail

int run_topic(int argc, char** argv);
void list_topics();

// C++20 string-literal carrier so we can pass topic ids as non-type template
// arguments. Stores the trailing '\0', strips it in view().
template <std::size_t N>
struct fixed_string {
    char data[N]{};

    constexpr fixed_string(const char (&str)[N]) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] = str[i];
        }
    }

    constexpr std::string_view view() const noexcept { return std::string_view{data, N - 1}; }
};

// Self-registering type: constructor inserts (Id, Fn) into the registry.
// One specialization per (Id, Fn) pair.
template <fixed_string Id, TopicFn Fn>
struct register_topic_t {
    register_topic_t() noexcept { detail::register_topic_impl(Id.view(), Fn); }
};

// Inline variable template. ODR-use a specialization from each topic .cpp
// (binding a reference is enough) and its constructor runs at static-init
// time, registering the topic. Pattern:
//
//   namespace {
//     int run(int, char**) { /* ... */ return 0; }
//     [[maybe_unused]] const auto& _ = ::learn::topic<"part.../foo", run>;
//   }
template <fixed_string Id, TopicFn Fn>
inline const register_topic_t<Id, Fn> topic{};

}  // namespace learn

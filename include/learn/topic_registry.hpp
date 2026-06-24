// LearnCpp topic registry.
//
// Every placeholder `.cpp` instantiates `learn::topic<"path/like/id", run>`,
// which is an inline variable template whose constructor inserts (id, run)
// into a global `std::map` at static-init time. `main()` then dispatches.
#pragma once

#include <cstddef>
#include <string_view>

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

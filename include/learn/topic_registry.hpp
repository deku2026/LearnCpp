// LearnCpp topic registry.
//
// Every placeholder `.cpp` registers an `int run(int, char**)` against a
// slash-separated id (e.g. "part2/stage01/section01/main_and_program_structure").
// At startup the registrations populate a `std::map` and `main()` dispatches.
#pragma once

#include <string_view>

namespace learn {

using TopicFn = int (*)(int argc, char** argv);

struct TopicRegistrar {
    TopicRegistrar(std::string_view id, TopicFn fn) noexcept;
};

int run_topic(int argc, char** argv);
void list_topics();

}  // namespace learn

#define LEARN_TOPIC_CAT_(a, b) a##b
#define LEARN_TOPIC_CAT(a, b) LEARN_TOPIC_CAT_(a, b)

// Use once per topic translation unit; expands to a file-scope registrar.
// __LINE__ (standard predefined) instead of __COUNTER__ (GNU extension that
// clang 22 flags as a C2y extension under -Wc2y-extensions / -Werror).
// Each .cpp has exactly one LEARN_TOPIC and the registrar lives in an
// anonymous namespace, so per-TU uniqueness is enough.
#define LEARN_TOPIC(id, fn)                                                      \
    static ::learn::TopicRegistrar LEARN_TOPIC_CAT(learn_topic_reg_, __LINE__) { \
        (id), (fn)                                                               \
    }

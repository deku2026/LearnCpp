// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string
// Topic id : part3/section01/std_string
//
// Covers: std::string as byte sequence, SSO, size/capacity, substr, find

#include "learn/topic_registry.hpp"

#include <string>

namespace {

void demo_basics() {
    std::string s = "Hello";
    s += ", World";
    LEARN_CHECK(s == "Hello, World");
    LEARN_CHECK(s.size() == 12);
    LEARN_CHECK(s.capacity() >= s.size());
    LEARN_CHECK(s[0] == 'H');
    LEARN_CHECK(s.front() == 'H');
    LEARN_CHECK(s.back() == 'd');
}

void demo_intermediate() {
    std::string s = "abc";
    s.append("def");
    s.insert(3, "X");
    LEARN_CHECK(s == "abcXdef");
    s.erase(3, 1);
    LEARN_CHECK(s == "abcdef");
    LEARN_CHECK(s.find("cd") == 2);
    LEARN_CHECK(s.substr(2, 3) == "cde");
    s.reserve(64);
    LEARN_CHECK(s.capacity() >= 64);
}

void demo_expert() {
    std::string short_s = "short";
    const char* data = short_s.data();
    LEARN_CHECK(data != nullptr);
    LEARN_CHECK(short_s.c_str()[short_s.size()] == '\0');
    std::string empty;
    LEARN_CHECK(empty.empty());
    empty.resize(3, 'z');
    LEARN_CHECK(empty == "zzz");
    // size() is bytes, not Unicode code points
    std::string utf8 = "hi";
    LEARN_CHECK(utf8.size() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string", run>;

}  // namespace

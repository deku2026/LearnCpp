// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string
// Topic id : part3/section01/std_string
//
// Covers: std::string as byte sequence, SSO, size/capacity, substr, find

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

void demo_basics() {
    std::string s = "Hello";
    s += ", World";
    assert(s == "Hello, World");
    assert(s.size() == 12);
    assert(s.capacity() >= s.size());
    assert(s[0] == 'H');
    assert(s.front() == 'H');
    assert(s.back() == 'd');
}

void demo_intermediate() {
    std::string s = "abc";
    s.append("def");
    s.insert(3, "X");
    assert(s == "abcXdef");
    s.erase(3, 1);
    assert(s == "abcdef");
    assert(s.find("cd") == 2);
    assert(s.substr(2, 3) == "cde");
    s.reserve(64);
    assert(s.capacity() >= 64);
}

void demo_expert() {
    std::string short_s = "short";
    const char* data = short_s.data();
    assert(data != nullptr);
    assert(short_s.c_str()[short_s.size()] == '\0');
    std::string empty;
    assert(empty.empty());
    empty.resize(3, 'z');
    assert(empty == "zzz");
    // size() is bytes, not Unicode code points
    std::string utf8 = "hi";
    assert(utf8.size() == 2);
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

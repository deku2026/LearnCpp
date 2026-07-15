// LearnCpp topic
// Doc      : 第2部分-阶段15-C++经典惯用法.md · 步骤 5（综合改写验收点）
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : legacy_to_modern_rewrite
// Topic id : part2/stage15/section04/legacy_to_modern_rewrite
//
// Acceptance: rewrite "raw pointer + magic return + manual free" into
//             unique_ptr + expected + RAII — the stage-15 capstone synthesis.
// Refs     : Effective Modern C++ / C++ Software Design / stage 4·5·9

#include "learn/topic_registry.hpp"

#include <cassert>
#include <expected>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

namespace {

// ---------- 5.1 legacy shape (C style, problems listed) ----------
// Not used at runtime for allocation paths that leak; kept as a catalog of smells.
struct LegacyConnection {
    char* buffer;
    int status;
};

// Smells (do not copy into new code):
//  - ownership via raw pointer + nullptr-as-error
//  - magic int return codes (-1 / 0)
//  - manual malloc/free (exception-path leaks, double-free)
//  - out-parameters for payload

// ---------- 5.2 modern rewrite ----------
class Connection {
    std::string buffer_;

public:
    explicit Connection(std::string_view host) : buffer_(64, '\0') {
        // RAII: resource acquired in ctor; no separate "init that can be forgotten"
        (void)host;
        buffer_ = "data-from-";
        buffer_ += host;
    }

    // expected: value or typed reason (no magic -1)
    std::expected<std::string, std::string> read_data() const {
        if (buffer_.empty()) {
            return std::unexpected(std::string{"empty buffer"});
        }
        return buffer_;
    }
};

// Ownership + failure reason in the type system
std::expected<std::unique_ptr<Connection>, std::string> open_connection(std::string_view host) {
    if (host.empty()) {
        return std::unexpected(std::string{"host is empty"});
    }
    return std::make_unique<Connection>(host);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [legacy_to_modern_rewrite] step 5 acceptance ===\n";

    std::cout << "== modern: railway open → read ==\n";
    {
        auto result = open_connection("example.com").and_then([](std::unique_ptr<Connection> conn) {
            // move unique_ptr into and_then; Connection destroyed when pipeline ends
            return conn->read_data();
        });
        assert(result);
        assert(result->find("example.com") != std::string::npos);
        std::cout << "  payload: " << *result << '\n';
        // no closeConnection — unique_ptr + ~Connection is the close
    }

    std::cout << "== modern: failure carries reason ==\n";
    {
        auto bad = open_connection("");
        assert(!bad);
        assert(bad.error() == "host is empty");
        std::cout << "  error: " << bad.error() << '\n';
    }

    std::cout << "== rewrite map (doc 5.3) ==\n";
    // | legacy                    | modern                         |
    // | raw owning pointer        | unique_ptr                     |
    // | nullptr means failure     | expected<T,E> with reason      |
    // | magic -1/0                | expected / enum class error    |
    // | malloc/free               | RAII members + smart pointers  |
    // | manual close (easy leak)  | destructor / unique_ptr        |
    // | out-parameters             | return expected<value, error>  |
    std::cout << "  raw*+magic → unique_ptr + expected + RAII\n";

    // Catalog only: sizeof legacy layout exists; we do not call malloc paths.
    assert(sizeof(LegacyConnection) >= sizeof(char*));
    std::cout << "  (legacy struct kept as size/smell catalog only)\n";

    std::cout << "[legacy_to_modern_rewrite] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/legacy_to_modern_rewrite", run>;

}  // namespace

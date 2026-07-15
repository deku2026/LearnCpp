// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §4.3
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : named_parameter_builder
// Topic id : part2/stage15/section04/named_parameter_builder
//
// Named parameters via builder / designated initializers.
// Motive : long optional argument lists are order-sensitive and error-prone.
// Modern : C++20 designated initializers for aggregates; fluent builders for
//          validation, defaults, and multi-step construction.
// Pitfall: builder mutability; forget build() checks; overkill for 1–2 params.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

// ---------- fluent builder ----------
class HttpRequest {
public:
    std::string url;
    std::string method = "GET";
    int timeout_sec = 30;
    bool verbose = false;

    void send() const {
        std::cout << "  " << method << ' ' << url << " timeout=" << timeout_sec << " verbose=" << std::boolalpha
                  << verbose << '\n';
    }
};

class HttpRequestBuilder {
    HttpRequest req_;

public:
    HttpRequestBuilder& url(std::string u) {
        req_.url = std::move(u);
        return *this;
    }
    HttpRequestBuilder& method(std::string m) {
        req_.method = std::move(m);
        return *this;
    }
    HttpRequestBuilder& timeout_sec(int t) {
        req_.timeout_sec = t;
        return *this;
    }
    HttpRequestBuilder& verbose(bool v) {
        req_.verbose = v;
        return *this;
    }

    [[nodiscard]] HttpRequest build() const {
        if (req_.url.empty()) {
            throw std::invalid_argument("url required");
        }
        if (req_.timeout_sec <= 0) {
            throw std::invalid_argument("timeout must be positive");
        }
        return req_;
    }
};

// ---------- C++20 designated initializers (lightweight named params) ----------
struct PlotOptions {
    int width = 800;
    int height = 600;
    std::string title = "plot";
    bool grid = true;
};

void render(const PlotOptions& o) {
    std::cout << "  plot " << o.width << 'x' << o.height << " \"" << o.title << "\" grid=" << std::boolalpha << o.grid
              << '\n';
}

// ---------- anti-pattern: positional soup ----------
void send_raw(const std::string& url, const std::string& method, int timeout, bool verbose) {
    std::cout << "  raw " << method << ' ' << url << ' ' << timeout << ' ' << verbose << '\n';
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== builder ==\n";
    auto req = HttpRequestBuilder{}.url("https://example.com/api").method("POST").timeout_sec(60).verbose(true).build();
    req.send();
    assert(req.method == "POST");
    assert(req.timeout_sec == 60);

    try {
        (void)HttpRequestBuilder{}.method("GET").build();
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << "  validation: " << e.what() << '\n';
    }

    std::cout << "== designated initializers ==\n";
    render({.width = 1024, .height = 768, .title = "demo", .grid = false});
    render({.title = "defaults for the rest"});  // width/height/grid keep defaults

    std::cout << "== positional contrast (easy to swap args) ==\n";
    send_raw("https://x", "GET", 30, false);
    // send_raw("https://x", "GET", false, 30); // type may still compile with promotions!

    std::cout << "prefer designated init for simple aggregates; builder when validation needed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/named_parameter_builder", run>;

}  // namespace

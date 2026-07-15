// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §2.1
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : nvi_non_virtual_interface
// Topic id : part2/stage15/section02/nvi_non_virtual_interface
//
// NVI (Non-Virtual Interface): public non-virtual API + private virtual hooks.
// Motive : base owns invariant workflow (pre/post, locking, logging); derived only
//          fills customization points and cannot bypass the skeleton.
// Modern : still the standard C++ form of Template Method; pairs well with final
//          public methods.
// Pitfall: overusing private virtuals when the whole operation should be freeform.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace {

class Document {
public:
    // Public non-virtual: fixed protocol. Derived cannot skip validate/log.
    void save() {
        std::cout << "  [lock]\n";
        validate();
        do_save();  // customization point
        ++save_count_;
        std::cout << "  [log] saved (#" << save_count_ << ")\n";
        std::cout << "  [unlock]\n";
    }

    int save_count() const noexcept { return save_count_; }

    virtual ~Document() = default;

protected:
    // Optional protected helpers shared by derived classes.
    void note(std::string_view msg) const { std::cout << "  note: " << msg << '\n'; }

private:
    void validate() const {
        std::cout << "  validate document\n";
        // invariants checked here once for every derived type
    }

    virtual void do_save() = 0;  // private virtual: only base may call it

    int save_count_ = 0;
};

class PdfDocument : public Document {
    void do_save() override {
        note("writing PDF stream");
        std::cout << "  save as PDF\n";
    }
};

class MarkdownDocument : public Document {
    void do_save() override {
        note("writing .md text");
        std::cout << "  save as Markdown\n";
    }
};

// Anti-pattern contrast: public virtual lets callers (and derived) skip protocol.
struct LooseDocument {
    virtual void save() { std::cout << "  loose save (no forced validate)\n"; }
    virtual ~LooseDocument() = default;
};

struct NaughtyPdf : LooseDocument {
    void save() override {
        // completely replaced protocol — base cannot enforce validate/log
        std::cout << "  naughty: skipped validate\n";
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== NVI: base owns the protocol ==\n";
    std::vector<std::unique_ptr<Document>> docs;
    docs.push_back(std::make_unique<PdfDocument>());
    docs.push_back(std::make_unique<MarkdownDocument>());

    for (auto& d : docs) {
        d->save();
        assert(d->save_count() == 1);
    }

    std::cout << "== public virtual can bypass protocol ==\n";
    NaughtyPdf bad;
    bad.save();

    std::cout << "use NVI when the algorithm skeleton is shared; plain virtual when not\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section02/nvi_non_virtual_interface", run>;

}  // namespace

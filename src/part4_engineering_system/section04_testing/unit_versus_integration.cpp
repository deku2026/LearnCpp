// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : unit_versus_integration
// Topic id : part4/section04/unit_versus_integration

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <map>
#include <string>
#include <string_view>

namespace {

struct LineItem {
    int unit_price{};
    int quantity{};
};

[[nodiscard]] constexpr int subtotal(LineItem item) noexcept {
    return item.unit_price * item.quantity;
}

class Inventory {
public:
    virtual ~Inventory() = default;
    [[nodiscard]] virtual int available(std::string_view sku) const = 0;
    virtual void remove(std::string_view sku, int quantity) = 0;
};

class InMemoryInventory final : public Inventory {
public:
    void stock(std::string sku, int quantity) { quantities_[std::move(sku)] = quantity; }

    [[nodiscard]] int available(std::string_view sku) const override {
        const auto found = quantities_.find(std::string{sku});
        return found == quantities_.end() ? 0 : found->second;
    }

    void remove(std::string_view sku, int quantity) override { quantities_.at(std::string{sku}) -= quantity; }

private:
    std::map<std::string, int, std::less<>> quantities_;
};

class CheckoutService {
public:
    explicit CheckoutService(Inventory& inventory) : inventory_(inventory) {}

    [[nodiscard]] bool checkout(std::string_view sku, int quantity) {
        if (quantity <= 0 || inventory_.available(sku) < quantity) {
            return false;
        }
        inventory_.remove(sku, quantity);
        return true;
    }

private:
    Inventory& inventory_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/unit_versus_integration"};

    // A unit test has no collaborators and pinpoints arithmetic rules quickly.
    static_assert(subtotal({25, 4}) == 100);
    LEARN_EXPECT_EQ(checks, subtotal({19, 3}), 57);

    // An integration test deliberately crosses the service/repository boundary.
    InMemoryInventory inventory;
    inventory.stock("cpp-book", 3);
    CheckoutService checkout{inventory};
    LEARN_EXPECT(checks, checkout.checkout("cpp-book", 2));
    LEARN_EXPECT_EQ(checks, inventory.available("cpp-book"), 1);
    LEARN_EXPECT(checks, !checkout.checkout("cpp-book", 2));
    LEARN_EXPECT_EQ(checks, inventory.available("cpp-book"), 1);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/unit_versus_integration", run>;

}  // namespace

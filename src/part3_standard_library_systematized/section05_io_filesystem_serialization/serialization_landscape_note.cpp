// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : serialization_landscape_note
// Topic id : part3/section05/serialization_landscape_note
// References: C++23 has no general serialization facility; [bit], [span]

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section05/serialization_landscape_note";
constexpr std::array<std::byte, 4> kMagic{std::byte{'L'}, std::byte{'C'}, std::byte{'P'}, std::byte{'P'}};
constexpr std::uint16_t kVersion = 1;
constexpr std::size_t kMaximumNameBytes = 1024;

struct Record {
    std::uint32_t id{};
    std::string name;

    friend bool operator==(const Record&, const Record&) = default;
};

enum class DecodeError { none, truncated, bad_magic, unsupported_version, invalid_length, trailing_data };

struct DecodeResult {
    std::optional<Record> value;
    DecodeError error{DecodeError::none};
};

void append_u16_be(std::vector<std::byte>& output, std::uint16_t value) {
    output.push_back(static_cast<std::byte>((value >> 8U) & 0xFFU));
    output.push_back(static_cast<std::byte>(value & 0xFFU));
}

void append_u32_be(std::vector<std::byte>& output, std::uint32_t value) {
    for (int shift : {24, 16, 8, 0}) {
        output.push_back(static_cast<std::byte>((value >> static_cast<unsigned int>(shift)) & 0xFFU));
    }
}

class Reader {
public:
    explicit Reader(std::span<const std::byte> bytes) : bytes_(bytes) {}

    bool read_u16_be(std::uint16_t& value) {
        if (remaining() < 2) {
            return false;
        }
        value = static_cast<std::uint16_t>((as_u8(bytes_[position_]) << 8U) | as_u8(bytes_[position_ + 1]));
        position_ += 2;
        return true;
    }

    bool read_u32_be(std::uint32_t& value) {
        if (remaining() < 4) {
            return false;
        }
        value = (as_u8(bytes_[position_]) << 24U) | (as_u8(bytes_[position_ + 1]) << 16U) |
                (as_u8(bytes_[position_ + 2]) << 8U) | as_u8(bytes_[position_ + 3]);
        position_ += 4;
        return true;
    }

    bool read_bytes(std::size_t count, std::span<const std::byte>& value) {
        if (count > remaining()) {
            return false;
        }
        value = bytes_.subspan(position_, count);
        position_ += count;
        return true;
    }

    [[nodiscard]] std::size_t remaining() const noexcept { return bytes_.size() - position_; }

private:
    static std::uint32_t as_u8(std::byte value) noexcept { return std::to_integer<std::uint32_t>(value); }

    std::span<const std::byte> bytes_;
    std::size_t position_{};
};

std::optional<std::vector<std::byte>> encode(const Record& record) {
    if (record.name.size() > kMaximumNameBytes || record.name.size() > 0xFFFFU) {
        return std::nullopt;
    }
    const std::size_t payload_size = 4 + 2 + record.name.size();
    std::vector<std::byte> bytes;
    bytes.reserve(kMagic.size() + 2 + 4 + payload_size);
    bytes.insert(bytes.end(), kMagic.begin(), kMagic.end());
    append_u16_be(bytes, kVersion);
    append_u32_be(bytes, static_cast<std::uint32_t>(payload_size));
    append_u32_be(bytes, record.id);
    append_u16_be(bytes, static_cast<std::uint16_t>(record.name.size()));
    for (const unsigned char character : record.name) {
        bytes.push_back(static_cast<std::byte>(character));
    }
    return bytes;
}

DecodeResult decode(std::span<const std::byte> bytes) {
    constexpr std::size_t header_size = 4 + 2 + 4;
    if (bytes.size() < header_size) {
        return {std::nullopt, DecodeError::truncated};
    }
    if (!std::ranges::equal(bytes.first<4>(), kMagic)) {
        return {std::nullopt, DecodeError::bad_magic};
    }

    Reader reader{bytes.subspan(4)};
    std::uint16_t version{};
    std::uint32_t payload_size{};
    if (!reader.read_u16_be(version) || !reader.read_u32_be(payload_size)) {
        return {std::nullopt, DecodeError::truncated};
    }
    if (version != kVersion) {
        return {std::nullopt, DecodeError::unsupported_version};
    }
    if (payload_size != reader.remaining()) {
        return {std::nullopt, DecodeError::invalid_length};
    }

    std::uint32_t id{};
    std::uint16_t name_size{};
    if (!reader.read_u32_be(id) || !reader.read_u16_be(name_size)) {
        return {std::nullopt, DecodeError::truncated};
    }
    if (name_size > kMaximumNameBytes || name_size > reader.remaining()) {
        return {std::nullopt, DecodeError::invalid_length};
    }
    std::span<const std::byte> encoded_name;
    if (!reader.read_bytes(name_size, encoded_name)) {
        return {std::nullopt, DecodeError::truncated};
    }
    if (reader.remaining() != 0) {
        return {std::nullopt, DecodeError::trailing_data};
    }

    std::string name;
    name.reserve(encoded_name.size());
    for (const std::byte value : encoded_name) {
        name.push_back(static_cast<char>(std::to_integer<unsigned char>(value)));
    }
    return {Record{id, std::move(name)}, DecodeError::none};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Record source{0x01020304U, "Ada"};
    const auto encoded = encode(source);
    LEARN_EXPECT(checks, encoded.has_value());
    if (!encoded) {
        return checks.result();
    }

    const DecodeResult decoded = decode(*encoded);
    LEARN_EXPECT(checks, decoded.value.has_value());
    if (decoded.value) {
        LEARN_EXPECT_EQ(checks, *decoded.value, source);
    }
    // Integers are encoded explicitly in network byte order, not by dumping a
    // struct representation (which would expose padding, endian and ABI).
    LEARN_EXPECT_EQ(checks, std::to_integer<unsigned int>((*encoded)[10]), 0x01U);
    LEARN_EXPECT_EQ(checks, std::to_integer<unsigned int>((*encoded)[13]), 0x04U);

    auto bad_magic = *encoded;
    bad_magic[0] = std::byte{'X'};
    LEARN_EXPECT_EQ(checks, decode(bad_magic).error, DecodeError::bad_magic);
    auto bad_version = *encoded;
    bad_version[5] = std::byte{2};
    LEARN_EXPECT_EQ(checks, decode(bad_version).error, DecodeError::unsupported_version);
    auto bad_length = *encoded;
    bad_length[9] = std::byte{0x7F};
    LEARN_EXPECT_EQ(checks, decode(bad_length).error, DecodeError::invalid_length);
    LEARN_EXPECT_EQ(checks, decode(std::span{encoded->data(), std::size_t{8}}).error, DecodeError::truncated);

    std::string oversized(kMaximumNameBytes + 1, 'x');
    LEARN_EXPECT(checks, !encode(Record{1, std::move(oversized)}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/serialization_landscape_note", run>;

}  // namespace

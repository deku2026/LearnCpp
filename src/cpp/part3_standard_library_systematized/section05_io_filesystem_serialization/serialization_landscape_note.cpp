// Topic    : 序列化版图（标准库无通用方案；手搓最小线协议 + 选型图）
// Doc      : 第3部分-标准库系统化.md · 库 5.4
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : serialization_landscape_note
// Topic id : part3/section05/serialization_landscape_note
// Refs     : nlohmann/json, protobuf, cereal, Boost.Serialization

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 教学用“玩具协议”：固定布局 Record{u32 id; u16 name_len; name bytes}
// 说明：真实项目应选库，不要手搓跨语言序列化。
struct Record {
    std::uint32_t id{};
    std::string name;
};

std::vector<std::byte> encode(const Record& r) {
    assert(r.name.size() <= 0xFFFF);
    std::vector<std::byte> out;
    out.resize(4 + 2 + r.name.size());
    const auto id = r.id;
    const auto n = static_cast<std::uint16_t>(r.name.size());
    std::memcpy(out.data(), &id, 4);
    std::memcpy(out.data() + 4, &n, 2);
    std::memcpy(out.data() + 6, r.name.data(), r.name.size());
    return out;
}

Record decode(std::span<const std::byte> bytes) {
    assert(bytes.size() >= 6);
    Record r;
    std::memcpy(&r.id, bytes.data(), 4);
    std::uint16_t n = 0;
    std::memcpy(&n, bytes.data() + 4, 2);
    assert(bytes.size() >= 6u + n);
    r.name.assign(reinterpret_cast<const char*>(bytes.data() + 6), n);
    return r;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [serialization_landscape_note] 标准库结论 ===\n";
    std::cout << "C++ standard library has NO general object serialization\n";
    std::cout << "no standard JSON / protobuf / reflective dump\n";

    std::cout << "=== 选型地图（探测打印） ===\n";
    std::cout << "JSON text     : nlohmann/json, RapidJSON, simdjson\n";
    std::cout << "schema/binary : protobuf, FlatBuffers, Cap'n Proto, MessagePack\n";
    std::cout << "C++-centric   : cereal, Boost.Serialization\n";
    std::cout << "Qt            : QDataStream, QJsonDocument (built-in)\n";

    std::cout << "=== 手搓最小二进制 round-trip（说明边界，非生产方案） ===\n";
    {
        Record in{42, "Ada"};
        const auto blob = encode(in);
        const auto out = decode(blob);
        assert(out.id == 42 && out.name == "Ada");
        std::cout << "toy protocol bytes=" << blob.size() << " id=" << out.id << " name=" << out.name << '\n';
        std::cout << "pitfalls: endianness, alignment, versioning, partial reads\n";
    }

    std::cout << "=== 文本线协议（调试友好，仍非通用序列化） ===\n";
    {
        std::ostringstream oss;
        oss << "id=" << 7 << ";name=" << "Bob";
        const auto line = oss.str();
        assert(line == "id=7;name=Bob");
        std::cout << "line protocol: " << line << '\n';
    }

    std::cout << "guidance: pick a library; use schema for cross-language evolution\n";
    std::cout << "[serialization_landscape_note] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/serialization_landscape_note", run>;

}  // namespace

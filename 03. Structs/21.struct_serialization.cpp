import std;
#include "structs_header.h"

// Simple POD struct for serialization
struct Header {
    uint32_t magic;
    uint16_t version;
    uint16_t length;
    uint32_t checksum;
};

void BasicSerialization() {
    std::cout << "=== Basic Serialization ===" << std::endl;

    Header h{0xDEADBEEF, 0x0100, 16, 0x12345678};

    std::cout << "Original struct:" << std::endl;
    std::cout << "  magic: 0x" << std::hex << h.magic << std::endl;
    std::cout << "  version: 0x" << h.version << std::endl;
    std::cout << "  length: " << std::dec << h.length << std::endl;
    std::cout << "  checksum: 0x" << std::hex << h.checksum << std::dec << std::endl;

    // Serialize to bytes
    std::vector<uint8_t> bytes(sizeof(Header));
    std::memcpy(bytes.data(), &h, sizeof(Header));

    std::cout << "\nSerialized bytes: ";
    for(uint8_t b : bytes) {
        std::cout << std::hex << static_cast<int>(b) << " ";
    }
    std::cout << std::dec << std::endl;

    // Deserialize from bytes
    Header restored;
    std::memcpy(&restored, bytes.data(), sizeof(Header));

    std::cout << "\nRestored struct:" << std::endl;
    std::cout << "  magic: 0x" << std::hex << restored.magic << std::endl;
    std::cout << "  version: 0x" << restored.version << std::dec << std::endl;
    std::cout << "  Restored successfully: " << (restored.magic == h.magic) << std::endl;
}

// Serialization with versioning
struct VersionedData {
    uint8_t version;
    // Version 1 fields
    int32_t int_field;
    // Version 2 added
    double double_field = 0.0;
    // Version 3 added
    std::string name;  // Note: std::string needs special handling!

    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> bytes;

        // Version
        bytes.push_back(version);

        // Version 1 fields
        for(size_t i = 0; i < sizeof(int32_t); ++i) {
            bytes.push_back(reinterpret_cast<const uint8_t*>(&int_field)[i]);
        }

        // Version 2 fields
        if(version >= 2) {
            for(size_t i = 0; i < sizeof(double); ++i) {
                bytes.push_back(reinterpret_cast<const uint8_t*>(&double_field)[i]);
            }
        }

        return bytes;
    }

    static VersionedData deserialize(const std::vector<uint8_t>& bytes) {
        VersionedData result;
        size_t offset = 0;

        result.version = bytes[offset++];

        std::memcpy(&result.int_field, bytes.data() + offset, sizeof(int32_t));
        offset += sizeof(int32_t);

        if(result.version >= 2 && offset + sizeof(double) <= bytes.size()) {
            std::memcpy(&result.double_field, bytes.data() + offset, sizeof(double));
        }

        return result;
    }
};

void VersionedSerialization() {
    std::cout << "\n=== Versioned Serialization ===" << std::endl;

    VersionedData v1_data;
    v1_data.version = 1;
    v1_data.int_field = 42;

    VersionedData v2_data;
    v2_data.version = 2;
    v2_data.int_field = 42;
    v2_data.double_field = 3.14;

    auto bytes1 = v1_data.serialize();
    auto bytes2 = v2_data.serialize();

    std::cout << "V1 serialized size: " << bytes1.size() << " bytes" << std::endl;
    std::cout << "V2 serialized size: " << bytes2.size() << " bytes" << std::endl;

    auto restored_v1 = VersionedData::deserialize(bytes1);
    auto restored_v2 = VersionedData::deserialize(bytes2);

    std::cout << "\nRestored v1: version=" << restored_v1.version
        << ", int=" << restored_v1.int_field << std::endl;
    std::cout << "Restored v2: version=" << restored_v2.version
        << ", int=" << restored_v2.int_field
        << ", double=" << restored_v2.double_field << std::endl;
}

// Binary protocol with header + payload
struct Packet {
    uint16_t type;
    uint16_t length;
    std::vector<uint8_t> payload;

    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> bytes;
        bytes.reserve(4 + payload.size());

        // Header
        bytes.push_back(type & 0xFF);
        bytes.push_back((type >> 8) & 0xFF);
        bytes.push_back(length & 0xFF);
        bytes.push_back((length >> 8) & 0xFF);

        // Payload
        bytes.insert(bytes.end(), payload.begin(), payload.end());

        return bytes;
    }

    static Packet deserialize(const uint8_t* data, size_t size) {
        Packet p;
        if(size < 4) return p;

        p.type = data[0] | (data[1] << 8);
        p.length = data[2] | (data[3] << 8);

        if(size >= 4 + p.length) {
            p.payload.assign(data + 4, data + 4 + p.length);
        }

        return p;
    }
};

void PacketSerialization() {
    std::cout << "\n=== Packet Serialization ===" << std::endl;

    Packet original;
    original.type = 0x0001;
    original.length = 5;
    original.payload = {1, 2, 3, 4, 5};

    auto bytes = original.serialize();
    std::cout << "Serialized packet (" << bytes.size() << " bytes): ";
    for(uint8_t b : bytes) {
        std::cout << std::hex << static_cast<int>(b) << " ";
    }
    std::cout << std::dec << std::endl;

    auto restored = Packet::deserialize(bytes.data(), bytes.size());
    std::cout << "\nRestored packet:" << std::endl;
    std::cout << "  type: 0x" << std::hex << restored.type << std::dec << std::endl;
    std::cout << "  length: " << restored.length << std::endl;
    std::cout << "  payload size: " << restored.payload.size() << std::endl;
}

// Endianness-aware serialization
void EndianAwareSerialization() {
    std::cout << "\n=== Endianness-Aware Serialization ===" << std::endl;

    uint32_t value = 0x12345678;

    // Little-endian (x86/x64 default)
    std::cout << "Host byte order: ";
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&value);
    for(int i = 0; i < 4; ++i) {
        std::cout << std::hex << static_cast<int>(bytes[i]) << " ";
    }
    std::cout << std::dec << std::endl;

    // Network byte order (big-endian)
    std::cout << "Network byte order: ";
    uint8_t network_bytes[4];
    network_bytes[0] = (value >> 24) & 0xFF;
    network_bytes[1] = (value >> 16) & 0xFF;
    network_bytes[2] = (value >> 8) & 0xFF;
    network_bytes[3] = value & 0xFF;

    for(int i = 0; i < 4; ++i) {
        std::cout << std::hex << static_cast<int>(network_bytes[i]) << " ";
    }
    std::cout << std::dec << std::endl;

    std::cout << "\nAlways specify byte order in protocols:" << std::endl;
    std::cout << "  - Network byte order (big-endian) is standard" << std::endl;
    std::cout << "  - Use htonl(), ntohl() for conversion" << std::endl;
}

// Serialization checklist
void SerializationChecklist() {
    std::cout << "\n=== Serialization Checklist ===" << std::endl;

    std::cout << "Before serializing a struct:" << std::endl;
    std::cout << "  1. Is it a POD type?" << std::endl;
    std::cout << "  2. No pointers? (pointers are process-specific)" << std::endl;
    std::cout << "  3. No virtual functions?" << std::endl;
    std::cout << "  4. Fixed-size members?" << std::endl;
    std::cout << "  5. Consider endianness" << std::endl;
    std::cout << "  6. Consider alignment/padding" << std::endl;
    std::cout << "  7. Version compatibility" << std::endl;

    std::cout << "\nFor complex types:" << std::endl;
    std::cout << "  - Use serialization library (protobuf, flatbuffers)" << std::endl;
    std::cout << "  - Or implement custom serialize/deserialize methods" << std::endl;
}
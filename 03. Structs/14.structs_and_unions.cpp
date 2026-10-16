import std;
#include "structs_header.h"

// Basic union
union Number {
    int i;
    float f;
    char bytes[4];
};

void BasicUnions() {
    std::cout << "=== Basic Unions ===" << std::endl;

    std::cout << "sizeof(Number) = " << sizeof(Number) << std::endl;

    Number n;
    n.i = 0x3F800000;  // IEEE 754 for 1.0f

    std::cout << "As int: " << n.i << std::endl;
    std::cout << "As float: " << n.f << std::endl;

    std::cout << "\nBytes: ";
    for(int i = 0; i < 4; ++i) {
        std::cout << std::hex << static_cast<int>(n.bytes[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}

// Union with constructor/destructor (C++11)
struct NonTrivial {
    std::string str;

    NonTrivial() { std::cout << "  NonTrivial constructed" << std::endl; }
    ~NonTrivial() { std::cout << "  NonTrivial destroyed" << std::endl; }
};

union MixedUnion {
    int i;
    double d;
    NonTrivial nt;

    // Must provide constructor/destructor if any member is non-trivial
    MixedUnion() : i(0) { std::cout << "  MixedUnion constructed (int)" << std::endl; }
    ~MixedUnion() { std::cout << "  MixedUnion destroyed" << std::endl; }
};

void UnionWithNonTrivial() {
    std::cout << "\n=== Unions with Non-Trivial Types ===" << std::endl;

    MixedUnion u;
    std::cout << "Active member is int: " << u.i << std::endl;

    // Must manually manage lifetime for non-trivial members
    // new (&u.nt) NonTrivial();
    // u.nt.~NonTrivial();
}

// Tagged union (type-safe)
struct Result {
    enum class Type { Success, Error, Loading } type;

    union {
        double value;
        int error_code;
    };

    static Result success(double val) {
        Result r;
        r.type = Type::Success;
        r.value = val;
        return r;
    }

    static Result error(int code) {
        Result r;
        r.type = Type::Error;
        r.error_code = code;
        return r;
    }
};

void TaggedUnionPattern() {
    std::cout << "\n=== Tagged Union Pattern ===" << std::endl;

    auto r1 = Result::success(42.0);
    auto r2 = Result::error(404);

    auto print_result = [](const Result& r) {
        switch(r.type) {
            case Result::Type::Success:
                std::cout << "Success: " << r.value << std::endl;
                break;
            case Result::Type::Error:
                std::cout << "Error: " << r.error_code << std::endl;
                break;
            default:
                std::cout << "Other state" << std::endl;
        }
        };

    print_result(r1);
    print_result(r2);
}

// Union for memory savings
struct PacketData {
    // Without union: 3 separate buffers = 3 * max_size bytes
    // With union: only max_size bytes
    static constexpr size_t MAX_SIZE = 1024;

    enum class Kind { Audio, Video, Text } kind;

    union {
        struct { short samples[MAX_SIZE / 2]; } audio;
        struct { uint8_t pixels[MAX_SIZE]; } video;
        struct { char text[MAX_SIZE]; } text_data;
    };

    size_t size() const {
        switch(kind) {
            case Kind::Audio: return sizeof(audio);
            case Kind::Video: return sizeof(video);
            case Kind::Text: return sizeof(text_data);
        }
        return 0;
    }
};

void MemorySavings() {
    std::cout << "\n=== Memory Savings with Unions ===" << std::endl;

    std::cout << "Without union:" << std::endl;
    std::cout << "  3 buffers of " << PacketData::MAX_SIZE << " bytes each" << std::endl;
    std::cout << "  = " << 3 * PacketData::MAX_SIZE << " bytes" << std::endl;

    std::cout << "\nWith union:" << std::endl;
    std::cout << "  1 buffer of " << PacketData::MAX_SIZE << " bytes" << std::endl;
    std::cout << "  = " << sizeof(PacketData) << " bytes" << std::endl;

    std::cout << "\nSavings: " << 3 * PacketData::MAX_SIZE - sizeof(PacketData)
        << " bytes" << std::endl;
}

// Union type punning (dangerous but sometimes used)
void TypePunning() {
    std::cout << "\n=== Type Punning with Unions ===" << std::endl;

    union Pun {
        float f;
        uint32_t i;
    };

    Pun p;
    p.f = 1.0f;

    std::cout << "float 1.0 as bits: 0x" << std::hex << p.i << std::dec << std::endl;

    // Set bits directly
    p.i = 0x40000000;  // 2.0f in IEEE 754
    std::cout << "0x40000000 as float: " << p.f << std::endl;

    std::cout << "\nNote: Union type punning is well-defined in C" << std::endl;
    std::cout << "In C++, it's technically UB but widely supported" << std::endl;
    std::cout << "Consider std::bit_cast (C++20) instead" << std::endl;
}

// std::variant as safer alternative
void SaferAlternative() {
    std::cout << "\n=== std::variant (Safer Alternative) ===" << std::endl;

    std::variant<int, double, std::string> v;

    v = 42;
    std::cout << "Currently holds int: " << std::get<int>(v) << std::endl;

    v = 3.14;
    std::cout << "Now holds double: " << std::get<double>(v) << std::endl;

    v = "hello";
    std::cout << "Now holds string: " << std::get<std::string>(v) << std::endl;

    // Type-safe access
    if(std::holds_alternative<std::string>(v)) {
        std::cout << "Confirmed: it's a string" << std::endl;
    }

    // std::visit for type-safe dispatch
    std::visit([](auto&& arg) {
        std::cout << "Value: " << arg << std::endl;
               }, v);

    std::cout << "\nstd::variant advantages:" << std::endl;
    std::cout << "  - Type-safe (knows what it holds)" << std::endl;
    std::cout << "  - No manual tag management" << std::endl;
    std::cout << "  - std::visit for pattern matching" << std::endl;
}
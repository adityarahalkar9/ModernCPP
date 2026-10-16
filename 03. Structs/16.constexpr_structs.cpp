import std;
#include "structs_header.h"

// constexpr struct
struct Vec3 {
    double x, y, z;

    constexpr Vec3(double x_, double y_, double z_)
        : x(x_), y(y_), z(z_) {
    }

    constexpr double dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    constexpr Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    constexpr double magnitude_squared() const {
        return x * x + y * y + z * z;
    }
};

void ConstexprStructs() {
    std::cout << "=== constexpr Structs ===" << std::endl;

    // Compile-time construction
    constexpr Vec3 a{1.0, 0.0, 0.0};
    constexpr Vec3 b{0.0, 1.0, 0.0};

    // Compile-time computation
    constexpr double dot_product = a.dot(b);
    static_assert(dot_product == 0.0);

    constexpr Vec3 c = a.cross(b);  // Should be (0, 0, 1)
    static_assert(c.z == 1.0);

    std::cout << "a = (" << a.x << ", " << a.y << ", " << a.z << ")" << std::endl;
    std::cout << "b = (" << b.x << ", " << b.y << ", " << b.z << ")" << std::endl;
    std::cout << "a · b = " << dot_product << std::endl;
    std::cout << "a × b = (" << c.x << ", " << c.y << ", " << c.z << ")" << std::endl;
}

// constexpr struct with more complex logic
struct Matrix2x2 {
    double a, b, c, d;

    constexpr Matrix2x2(double a_, double b_, double c_, double d_)
        : a(a_), b(b_), c(c_), d(d_) {
    }

    constexpr Matrix2x2 multiply(const Matrix2x2& other) const {
        return Matrix2x2(
            a * other.a + b * other.c,
            a * other.b + b * other.d,
            c * other.a + d * other.c,
            c * other.b + d * other.d
        );
    }

    constexpr double determinant() const {
        return a * d - b * c;
    }

    constexpr Matrix2x2 identity() {
        return Matrix2x2(1, 0, 0, 1);
    }
};

void ConstexprMatrix() {
    std::cout << "\n=== constexpr Matrix Operations ===" << std::endl;

    constexpr Matrix2x2 m1{1, 2, 3, 4};
    constexpr Matrix2x2 m2{5, 6, 7, 8};

    constexpr Matrix2x2 product = m1.multiply(m2);
    constexpr double det = product.determinant();

    static_assert(det != 0.0);  // Compile-time verification

    std::cout << "m1 determinant: " << m1.determinant() << std::endl;
    std::cout << "m2 determinant: " << m2.determinant() << std::endl;
    std::cout << "m1*m2 determinant: " << det << std::endl;
}

// Compile-time data tables
struct PrimeTable {
    std::array<int, 20> primes{};

    constexpr PrimeTable() {
        int count = 0;
        for(int candidate = 2; count < 20; ++candidate) {
            bool is_prime = true;
            for(int i = 2; i * i <= candidate; ++i) {
                if(candidate % i == 0) {
                    is_prime = false;
                    break;
                }
            }
            if(is_prime) {
                primes[count++] = candidate;
            }
        }
    }

    constexpr int get(int index) const {
        return primes[index];
    }
};

void CompileTimeDataGeneration() {
    std::cout << "\n=== Compile-Time Data Generation ===" << std::endl;

    constexpr PrimeTable table;

    std::cout << "First 20 primes: ";
    for(int i = 0; i < 20; ++i) {
        std::cout << table.get(i) << " ";
    }
    std::cout << std::endl;

    // Compile-time verification
    static_assert(table.get(0) == 2);
    static_assert(table.get(9) == 29);
    static_assert(table.get(19) == 71);
}

// consteval structs (C++20)
struct CompileTimeOnly {
    int value;

    consteval CompileTimeOnly(int v) : value(v) {}
    consteval int get() const { return value; }
};

void ConstevalStructs() {
    std::cout << "\n=== consteval Structs (C++20) ===" << std::endl;

    // Can only be used at compile time
    constexpr CompileTimeOnly cto{42};
    static_assert(cto.get() == 42);

    std::cout << "Compile-time value: " << cto.get() << std::endl;

    std::cout << "\nconsteval means:" << std::endl;
    std::cout << "  - Function/constructor MUST be evaluated at compile time" << std::endl;
    std::cout << "  - Cannot be called at runtime" << std::endl;
    std::cout << "  - Ensures values are compile-time constants" << std::endl;
}

// constexpr with std::string (C++20)
void ConstexprString() {
    std::cout << "\n=== constexpr with Strings ===" << std::endl;

    std::cout << "C++20 allows constexpr std::string in limited contexts" << std::endl;
    std::cout << "But strings can't persist to runtime" << std::endl;

    // This works in C++20:
    // constexpr auto str = std::string("hello");
    // But 'str' can only be used in constant expressions

    std::cout << "\nFor compile-time strings, consider:" << std::endl;
    std::cout << "  - constexpr char arrays" << std::endl;
    std::cout << "  - String literals" << std::endl;
    std::cout << "  - std::string_view (constexpr in C++20)" << std::endl;
}
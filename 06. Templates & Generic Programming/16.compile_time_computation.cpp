import std;
#include "templates_header.h"

// Compile-time array generation
template <size_t N>
constexpr std::array<int, N> MakeSquares() {
    std::array<int, N> result{};
    for(size_t i = 0; i < N; ++i) {
        result[i] = static_cast<int>(i * i);
    }
    return result;
}

void CompileTimeArrays() {
    std::cout << "=== Compile-Time Arrays ===" << std::endl;

    constexpr auto squares = MakeSquares<10>();
    static_assert(squares[0] == 0);
    static_assert(squares[5] == 25);
    static_assert(squares[9] == 81);

    std::cout << "Squares: ";
    for(int sq : squares) {
        std::cout << sq << " ";
    }
    std::cout << std::endl;
}

// Compile-time string manipulation
constexpr size_t StringLength(const char* str) {
    size_t len = 0;
    while(str[len] != '\0') ++len;
    return len;
}

void CompileTimeStrings() {
    std::cout << "\n=== Compile-Time Strings ===" << std::endl;

    constexpr size_t len = StringLength("Hello, World!");
    static_assert(len == 13);

    std::cout << "Length of \"Hello, World!\": " << len << " (compile-time)" << std::endl;
}

// Compile-time prime checking
constexpr bool IsPrime(int n) {
    if(n < 2) return false;
    for(int i = 2; i * i <= n; ++i) {
        if(n % i == 0) return false;
    }
    return true;
}

// Generate prime table at compile time
template <size_t N>
constexpr std::array<int, N> MakePrimes() {
    std::array<int, N> primes{};
    size_t count = 0;
    for(int candidate = 2; count < N; ++candidate) {
        if(IsPrime(candidate)) {
            primes[count++] = candidate;
        }
    }
    return primes;
}

void CompileTimePrimes() {
    std::cout << "\n=== Compile-Time Primes ===" << std::endl;

    constexpr auto primes = MakePrimes<20>();
    static_assert(primes[0] == 2);
    static_assert(primes[1] == 3);
    static_assert(primes[19] == 71);

    std::cout << "First 20 primes: ";
    for(int p : primes) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
}

// Compile-time mathematical constants
template <int N>
constexpr double ComputePi() {
    // Leibniz formula: π/4 = 1 - 1/3 + 1/5 - 1/7 + ...
    double result = 0.0;
    for(int i = 0; i < N; ++i) {
        result += (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
    }
    return result * 4.0;
}

void CompileTimeMath() {
    std::cout << "\n=== Compile-Time Math ===" << std::endl;

    constexpr double pi = ComputePi<10000>();
    std::cout << "π ≈ " << pi << " (computed at compile time)" << std::endl;
    std::cout << "Actual π = 3.14159265358979..." << std::endl;
}

// Compile-time sorting
template <size_t N>
constexpr std::array<int, N> SortArray(std::array<int, N> arr) {
    // Bubble sort at compile time
    for(size_t i = 0; i < N; ++i) {
        for(size_t j = 0; j + 1 < N - i; ++j) {
            if(arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}

void CompileTimeSorting() {
    std::cout << "\n=== Compile-Time Sorting ===" << std::endl;

    constexpr std::array<int, 8> unsorted = {5, 2, 8, 1, 9, 3, 7, 4};
    constexpr auto sorted = SortArray(unsorted);

    static_assert(sorted[0] == 1);
    static_assert(sorted[7] == 9);

    std::cout << "Sorted: ";
    for(int val : sorted) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}